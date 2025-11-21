/**
 * @file Display.h
 * @brief Gestion de l'écran LCD 3.5" avec AXS15231B
 */

#ifndef WAVESHARE_DISPLAY_H
#define WAVESHARE_DISPLAY_H

#include <Arduino.h>
#include "../config/features.h"

#ifdef FEATURE_DISPLAY_ENABLED

#include <Arduino_GFX_Library.h>
#include "../config/pins_definitions.h"
#include "../config/display_config.h"

class WaveshareDisplay {
public:
    WaveshareDisplay();
    ~WaveshareDisplay();
    
    // ========================================================================
    // INITIALISATION
    // ========================================================================
    
    /**
     * @brief Initialise l'écran LCD
     * @return true si succès, false sinon
     */
    bool begin();
    
    /**
     * @brief Termine et libère les ressources
     */
    void end();
    
    // ========================================================================
    // CONTRÔLE DE BASE
    // ========================================================================
    
    /**
     * @brief Active/désactive l'écran
     * @param on true pour activer, false pour désactiver
     */
    void power(bool on);
    
    /**
     * @brief Efface l'écran avec une couleur
     * @param color Couleur RGB565
     */
    void clear(uint16_t color = COLOR_BLACK);
    
    /**
     * @brief Définit la rotation de l'écran
     * @param rotation 0-3 (0=portrait, 1=paysage, 2=portrait inversé, 3=paysage inversé)
     */
    void setRotation(uint8_t rotation);
    
    /**
     * @brief Obtient la rotation actuelle
     * @return Rotation actuelle (0-3)
     */
    uint8_t getRotation();
    
    // ========================================================================
    // RÉTROÉCLAIRAGE
    // ========================================================================
    
    /**
     * @brief Définit la luminosité du rétroéclairage
     * @param brightness Luminosité (0-100)
     */
    void setBrightness(uint8_t brightness);
    
    /**
     * @brief Obtient la luminosité actuelle
     * @return Luminosité (0-100)
     */
    uint8_t getBrightness();
    
    /**
     * @brief Augmente la luminosité
     */
    void increaseBrightness();
    
    /**
     * @brief Diminue la luminosité
     */
    void decreaseBrightness();
    
    // ========================================================================
    // DESSIN
    // ========================================================================
    
    /**
     * @brief Dessine un pixel
     * @param x Coordonnée X
     * @param y Coordonnée Y
     * @param color Couleur RGB565
     */
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    
    /**
     * @brief Dessine une ligne
     */
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    
    /**
     * @brief Dessine un rectangle
     */
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    
    /**
     * @brief Dessine un rectangle rempli
     */
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    
    /**
     * @brief Dessine un cercle
     */
    void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
    
    /**
     * @brief Dessine un cercle rempli
     */
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
    
    /**
     * @brief Dessine un triangle
     */
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
                      int16_t x2, int16_t y2, uint16_t color);
    
    /**
     * @brief Dessine un triangle rempli
     */
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, uint16_t color);
    
    /**
     * @brief Dessine un rectangle arrondi
     */
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, 
                       int16_t r, uint16_t color);
    
    /**
     * @brief Dessine un rectangle arrondi rempli
     */
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                       int16_t r, uint16_t color);
    
    // ========================================================================
    // TEXTE
    // ========================================================================
    
    /**
     * @brief Définit la couleur du texte
     * @param color Couleur RGB565
     */
    void setTextColor(uint16_t color);
    
    /**
     * @brief Définit la couleur du texte avec fond
     */
    void setTextColor(uint16_t fgColor, uint16_t bgColor);
    
    /**
     * @brief Définit la taille du texte
     * @param size Taille (1-8)
     */
    void setTextSize(uint8_t size);
    
    /**
     * @brief Positionne le curseur pour le texte
     */
    void setCursor(int16_t x, int16_t y);
    
    /**
     * @brief Écrit du texte à la position du curseur
     */
    void print(const char* text);
    void print(int number);
    void print(float number, int decimals = 2);
    
    /**
     * @brief Écrit du texte avec retour à la ligne
     */
    void println(const char* text);
    void println(int number);
    void println(float number, int decimals = 2);
    
    // ========================================================================
    // IMAGES
    // ========================================================================
    
    /**
     * @brief Dessine une image bitmap
     * @param x Position X
     * @param y Position Y
     * @param bitmap Pointeur vers les données bitmap
     * @param w Largeur
     * @param h Hauteur
     * @param color Couleur (pour bitmap monochrome)
     */
    void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, 
                    int16_t w, int16_t h, uint16_t color);
    
    /**
     * @brief Dessine une image RGB565
     */
    void drawRGBBitmap(int16_t x, int16_t y, const uint16_t* bitmap,
                       int16_t w, int16_t h);
    
    // ========================================================================
    // UTILITAIRES
    // ========================================================================
    
    /**
     * @brief Obtient la largeur de l'écran
     */
    int16_t width();
    
    /**
     * @brief Obtient la hauteur de l'écran
     */
    int16_t height();
    
    /**
     * @brief Convertit RGB888 en RGB565
     */
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
    
    /**
     * @brief Obtient le pointeur vers l'objet GFX
     */
    Arduino_GFX* getGFX() { return _gfx; }
    
    /**
     * @brief Vérifie si l'écran est initialisé
     */
    bool isInitialized() { return _initialized; }

private:
    Arduino_DataBus* _bus;
    Arduino_GFX* _display;
    Arduino_Canvas* _gfx;
    
    bool _initialized;
    uint8_t _brightness;
    uint8_t _rotation;
    
    void initBacklight();
};

#endif // FEATURE_DISPLAY_ENABLED

#endif // WAVESHARE_DISPLAY_H
