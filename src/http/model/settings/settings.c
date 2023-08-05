#include <nvs.h>
#include "cJSON.h"

#define NVS_NAMESPACE "settings"
#define JSON_KEY "settings_json"

const char* get_default_settings() {
    return
            "{"
            "  \"dateTime\": 0,"
            "  \"silentTime\": {"
            "    \"startHour\": 0,"
            "    \"startMinute\": 0,"
            "    \"endHour\": 0,"
            "    \"endMinute\": 0"
            "  },"
            "  \"soilMoisture\": 1,"
            "  \"checkFrequencyByDay\": 1,"
            "  \"volumeWaterPerTimeML\": 1"
            "}";
}

bool is_settings_valid(const char *json_str) {
    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL) {
        return false;
    }

    if (!cJSON_IsNumber(cJSON_GetObjectItem(json, "dateTime"))) {
        cJSON_Delete(json);
        return false;
    }

    if (!cJSON_IsObject(cJSON_GetObjectItem(json, "silentTime"))) {
        cJSON_Delete(json);
        return false;
    }

    cJSON *silent_time = cJSON_GetObjectItem(json, "silentTime");
    if (!cJSON_IsNumber(cJSON_GetObjectItem(silent_time, "startHour")) ||
        !cJSON_IsNumber(cJSON_GetObjectItem(silent_time, "startMinute")) ||
        !cJSON_IsNumber(cJSON_GetObjectItem(silent_time, "endHour")) ||
        !cJSON_IsNumber(cJSON_GetObjectItem(silent_time, "endMinute"))) {
        cJSON_Delete(json);
        return false;
    }

    if (!cJSON_IsNumber(cJSON_GetObjectItem(json, "soilMoisture"))) {
        cJSON_Delete(json);
        return false;
    }

    if (!cJSON_IsNumber(cJSON_GetObjectItem(json, "checkFrequencyByDay"))) {
        cJSON_Delete(json);
        return false;
    }

    if (!cJSON_IsNumber(cJSON_GetObjectItem(json, "volumeWaterPerTimeML"))) {
        cJSON_Delete(json);
        return false;
    }

    cJSON_Delete(json);

    return true;
}

bool save_settings_to_nvs(const char *json_str) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return false;
    }

    err = nvs_set_str(nvs_handle, JSON_KEY, json_str);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return false;
    }

    nvs_close(nvs_handle);
    return true;
}

char* read_settings_from_nvs() {
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (ret != ESP_OK) {
        return NULL;
    }

    size_t required_size;
    ret = nvs_get_blob(nvs_handle, JSON_KEY, NULL, &required_size);
    if (ret != ESP_OK) {
        nvs_close(nvs_handle);
        return NULL;
    }

    char *json_str = (char *) malloc(required_size);
    if (json_str == NULL) {
        nvs_close(nvs_handle);
        return NULL;
    }

    ret = nvs_get_blob(nvs_handle, JSON_KEY, json_str, &required_size);
    if (ret != ESP_OK) {
        free(json_str);
        nvs_close(nvs_handle);
        return NULL;
    }

    nvs_close(nvs_handle);

    return json_str;
}