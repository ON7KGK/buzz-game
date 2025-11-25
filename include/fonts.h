/**
 * @file fonts.h
 * @brief Inclusion des polices Adafruit GFX utilisées
 *
 * @author SPARKOH! - Michaël
 * @date 2025
 */

#ifndef FONTS_H
#define FONTS_H

// Polices standards (9-24pt) déjà incluses dans Display.h via Adafruit GFX Library
// On inclut uniquement les grandes polices (48, 60, 72pt) qui ne sont pas dans Adafruit

// Police 72pt pour affichage du compteur
#include <FreeSansBold72pt7b.h>

// Police 14pt avec accents français pour instructions courtes
#include "../Fonts/FreeSansBold14ptAccents7b.h"

// Police 18pt avec accents français pour les consignes
#include "../Fonts/FreeSansBold18ptAccents7b.h"

#endif // FONTS_H
