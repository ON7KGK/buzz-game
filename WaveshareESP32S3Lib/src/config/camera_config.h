/**
 * @file camera_config.h
 * @brief Configuration de la caméra OV5640 (uniquement pour version -C)
 */

#ifndef WAVESHARE_CAMERA_CONFIG_H
#define WAVESHARE_CAMERA_CONFIG_H

#include "pins_definitions.h"

#ifdef FEATURE_CAMERA_ENABLED

// ============================================================================
// CONFIGURATION CAMÉRA OV5640
// ============================================================================

// Modèle de caméra
#define CAMERA_MODEL                CAMERA_MODEL_OV5640

// ============================================================================
// RÉSOLUTION
// ============================================================================

// Résolution par défaut
// Options: FRAMESIZE_QVGA (320x240), FRAMESIZE_VGA (640x480), 
//          FRAMESIZE_SVGA (800x600), FRAMESIZE_XGA (1024x768),
//          FRAMESIZE_HD (1280x720), FRAMESIZE_UXGA (1600x1200),
//          FRAMESIZE_QXGA (2048x1536), FRAMESIZE_QSXGA (2560x1920)
#define CAMERA_DEFAULT_FRAMESIZE    FRAMESIZE_VGA  // 640x480

// Qualité JPEG (0-63, plus bas = meilleure qualité)
#define CAMERA_JPEG_QUALITY         12

// ============================================================================
// FORMAT DE PIXEL
// ============================================================================

// Format de pixel
// Options: PIXFORMAT_RGB565, PIXFORMAT_YUV422, PIXFORMAT_GRAYSCALE,
//          PIXFORMAT_JPEG, PIXFORMAT_RGB888, PIXFORMAT_RAW
#define CAMERA_PIXEL_FORMAT         PIXFORMAT_JPEG

// ============================================================================
// FRAMERATE
// ============================================================================

// Fréquence d'image par défaut (FPS)
// Valeurs typiques: 5, 10, 15, 20, 25
#define CAMERA_DEFAULT_FPS          15

// XCLK Frequency (Hz)
// 10MHz ou 20MHz pour OV5640
#define CAMERA_XCLK_FREQ            20000000

// ============================================================================
// BUFFERS
// ============================================================================

// Nombre de frame buffers
// 1 = moins de RAM mais latence possible
// 2 = meilleur pour streaming
#define CAMERA_FB_COUNT             2

// Utiliser PSRAM pour les buffers
#define CAMERA_USE_PSRAM            true

// Taille du buffer de capture
#define CAMERA_GRAB_MODE            CAMERA_GRAB_LATEST

// ============================================================================
// PARAMÈTRES D'IMAGE
// ============================================================================

// Luminosité (-2 à 2)
#define CAMERA_DEFAULT_BRIGHTNESS   0

// Contraste (-2 à 2)
#define CAMERA_DEFAULT_CONTRAST     0

// Saturation (-2 à 2)
#define CAMERA_DEFAULT_SATURATION   0

// Effet spécial
// 0=Aucun, 1=Négatif, 2=N&B, 3=Rouge, 4=Vert, 5=Bleu, 6=Sépia
#define CAMERA_DEFAULT_SPECIAL_EFFECT   0

// Balance des blancs automatique
#define CAMERA_WHITEBAL             true

// AWB Gain
#define CAMERA_AWB_GAIN             true

// Gain automatique
#define CAMERA_GAIN_CTRL            true

// Exposition automatique
#define CAMERA_EXPOSURE_CTRL        true

// Miroir horizontal
#define CAMERA_HMIRROR              false

// Miroir vertical
#define CAMERA_VFLIP                false

// ============================================================================
// STREAMING
// ============================================================================

// Activer le streaming HTTP
#define CAMERA_ENABLE_HTTP_STREAM   true

// Port du serveur de streaming
#define CAMERA_STREAM_PORT          81

// Chemin du stream
#define CAMERA_STREAM_PATH          "/stream"

// ============================================================================
// CAPTURE D'IMAGE
// ============================================================================

// Dossier de sauvegarde des photos
#define CAMERA_PHOTO_DIR            "/sdcard/photos"

// Format du nom de fichier
// %Y=année, %m=mois, %d=jour, %H=heure, %M=minute, %S=seconde
#define CAMERA_PHOTO_FILENAME       "IMG_%Y%m%d_%H%M%S.jpg"

// Nombre maximum de photos à garder
#define CAMERA_MAX_PHOTOS           100

// ============================================================================
// DÉTECTION DE MOUVEMENT
// ============================================================================

// Activer la détection de mouvement
#define CAMERA_MOTION_DETECT        false

// Seuil de détection (0-100)
#define CAMERA_MOTION_THRESHOLD     30

// ============================================================================
// PERFORMANCE
// ============================================================================

// Priorité de la tâche caméra
#define CAMERA_TASK_PRIORITY        2

// Taille de la pile de la tâche
#define CAMERA_TASK_STACK_SIZE      4096

// Core sur lequel exécuter la tâche (0 ou 1)
#define CAMERA_TASK_CORE            1

#endif // FEATURE_CAMERA_ENABLED

#endif // WAVESHARE_CAMERA_CONFIG_H
