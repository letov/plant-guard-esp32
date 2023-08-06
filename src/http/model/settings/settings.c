#include <nvs.h>
#include <esp_log.h>
#include "cJSON.h"

#define NVS_NAMESPACE "settings"
#define JSON_KEY "settings_json"
#define TAG "SETTINGS"

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
        ESP_LOGI(TAG, "open settings storage...FAILED");
        return false;
    }

    err = nvs_set_str(nvs_handle, JSON_KEY, json_str);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "set settings...FAILED");
        nvs_close(nvs_handle);
        return false;
    }

    ESP_LOGI(TAG, "set settings...OK");
    nvs_close(nvs_handle);
    return true;
}

char* read_settings_from_nvs() {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "open settings storage...FAILED");
        return NULL;
    }

    size_t settings_size;
    err = nvs_get_str(nvs_handle, JSON_KEY, NULL, &settings_size);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "get settings size...FAILED");
        nvs_close(nvs_handle);
        return NULL;
    }

    char *settings = (char *) malloc(settings_size);
    if (settings == NULL) {
        ESP_LOGI(TAG, "malloc settings buffer...FAILED");
        nvs_close(nvs_handle);
        return NULL;
    }

    err = nvs_get_str(nvs_handle, JSON_KEY, settings, &settings_size);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "get settings...FAILED");
        free(settings);
        nvs_close(nvs_handle);
        return NULL;
    }

    nvs_close(nvs_handle);

    ESP_LOGI(TAG, "get settings...OK");
    return settings;
}