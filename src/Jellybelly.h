#ifndef JELLYBELLY_H
#define JELLYBELLY_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Jellybelly {
private:
    std::string api_base = "https://jellybellywikiapi.onrender.com/api";
    http_client_config client_config;
    
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("jellybellywikiapi.onrender.com"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                // Кодируем значения параметров
                auto encoded_value = web::uri::encode_data_string(utility::conversions::to_string_t(param.second));
                query += param.first + "=" + utility::conversions::to_utf8string(encoded_value);
                first = false;
            }
        }
        return query;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Jellybelly(){
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> get_beans_by_id(int id) {
        return make_api_call("/Beans/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> get_beans(int pageIndex=0,int pageSize=0,const std::string& groupName="",const std::string& flavorName="",const std::string& colorGroup="",int sku=0,bool glutenFree=false,bool sugarFree=false,bool seasonal=false,bool kosher=false) {
        std::map<std::string, std::string> params;
        if (pageIndex>0) params["pageIndex"] = std::to_string(pageIndex);
        if (pageSize>0) params["pageSize"] = std::to_string(pageSize);
        if (!groupName.empty()) params["groupName"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(groupName)));
        if (!flavorName.empty()) params["flavorName"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(flavorName)));
        if (!colorGroup.empty()) params["colorGroup"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(colorGroup)));
        if (sku>0) params["sku"] = std::to_string(sku);
        if (glutenFree!=false) params["glutenFree"] = glutenFree;
        if (sugarFree!=false) params["sugarFree"] = sugarFree;
        if (seasonal!=false) params["seasonal"] = seasonal;
        if (kosher!=false) params["kosher"] = kosher;
        return make_api_call("/Beans/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> get_combinations_by_id(int id) {
        return make_api_call("/Combinations/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> get_combinations(int pageIndex=0,int pageSize=0,const std::string& name="") {
        std::map<std::string, std::string> params;
        if (pageIndex>0) params["pageIndex"] = std::to_string(pageIndex);
        if (pageSize>0) params["pageSize"] = std::to_string(pageSize);
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/Combinations/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> get_facts_by_id(int id) {
        return make_api_call("/Facts/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> get_facts(int pageIndex=0,int pageSize=0,const std::string& title="") {
        std::map<std::string, std::string> params;
        if (pageIndex>0) params["pageIndex"] = std::to_string(pageIndex);
        if (pageSize>0) params["pageSize"] = std::to_string(pageSize);
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/Facts/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> get_milestones_by_id(int id) {
        return make_api_call("/MileStones/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> get_milestones(int year=0,int pageIndex=0,int pageSize=0) {
        std::map<std::string, std::string> params;
        if (year>0) params["year"] = std::to_string(year);
        if (pageIndex>0) params["pageIndex"] = std::to_string(pageIndex);
        if (pageSize>0) params["pageSize"] = std::to_string(pageSize);
        return make_api_call("/MileStones/" + build_query_params(params),"GET");
    }

    pplx::task<json::value> get_recipes_by_id(int id) {
        return make_api_call("/Recipes/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> get_recipes(int pageIndex=0,int pageSize=0,const std::string& name="") {
        std::map<std::string, std::string> params;
        if (pageIndex>0) params["pageIndex"] = std::to_string(pageIndex);
        if (pageSize>0) params["pageSize"] = std::to_string(pageSize);
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/Recipes/" + build_query_params(params),"GET");
    }

};

#endif
