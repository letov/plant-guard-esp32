#ifndef PLANT_GUARD_ESP32_SETTINGS_H
#define PLANT_GUARD_ESP32_SETTINGS_H

const char* get_default_settings();
char* read_settings_from_nvs();
bool is_settings_valid(const char *json_str);
bool save_settings_to_nvs(const char *json_str);

#endif //PLANT_GUARD_ESP32_SETTINGS_H
