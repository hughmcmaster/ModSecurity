/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include "operators/geo_lookup.h"

#include <GeoIPCity.h>

#include <string>
#include <functional>

#include "operators/operator.h"
#include "utils/geo_lookup.h"


namespace ModSecurity {
namespace operators {


bool GeoLookup::evaluate(Assay *assay, const std::string &exp) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    GeoIPRecord *gir;

    bool ret = Utils::GeoLookup::getInstance().lookup(exp, &gir,
        std::bind(&GeoLookup::debug, this, assay, _1, _2));

    if (ret && gir) {
        if (gir->country_code) {
            assay->store_variable("GEO:COUNTRY_CODE", gir->country_code);
        }
        if (gir->country_code3) {
            assay->store_variable("GEO:COUNTRY_CODE3", gir->country_code3);
        }
        if (gir->country_name) {
            assay->store_variable("GEO:COUNTRY_NAME", gir->country_name);
        }
        if (gir->continent_code) {
            assay->store_variable("GEO:COUNTRY_CONTINENT",
                gir->continent_code);
        }
        if (gir->country_code && gir->region) {
            assay->store_variable("GEO:REGION",
                GeoIP_region_name_by_code(gir->country_code, gir->region));
        }
        if (gir->city) {
            assay->store_variable("GEO:CITY", gir->city);
        }
        if (gir->postal_code) {
            assay->store_variable("GEO:POSTAL_CODE", gir->postal_code);
        }
        if (gir->latitude) {
            assay->store_variable("GEO:LATITUDE",
                std::to_string(gir->latitude));
        }
        if (gir->longitude) {
            assay->store_variable("GEO:LONGITUDE",
                std::to_string(gir->longitude));
        }
        if (gir->metro_code) {
            assay->store_variable("GEO:DMA_CODE",
                std::to_string(gir->metro_code));
        }
        if (gir->area_code) {
            assay->store_variable("GEO:AREA_CODE",
                std::to_string(gir->area_code));
        }

        GeoIPRecord_delete(gir);
    }

    return ret;
}

GeoLookup::GeoLookup(std::string op, std::string param,
bool negation)
    : Operator() {
    this->op = op;
    this->param = param;
}

}  // namespace operators
}  // namespace ModSecurity