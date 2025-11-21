Import("env")

# Exclure le fichier Arduino_ESP32RGBPanel.cpp qui cause des problèmes de compilation
# Ce fichier n'est pas nécessaire pour notre usage (QSPI seulement)
env.Append(
    SRC_FILTER=[
        "+<*>",
        "-<.git/>",
        "-<.svn/>",
        "-<databus/Arduino_ESP32RGBPanel.cpp>"
    ]
)

print("Build script loaded: Excluding Arduino_ESP32RGBPanel.cpp")
