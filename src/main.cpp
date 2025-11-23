/*
 * ESP32-S3-Touch-LCD-3.5B - Compteur avec POLICES ADAFRUIT
 *
 * ⚠️ NÉCESSITE LA BIBLIOTHÈQUE ADAFRUIT GFX
 * Installation : Croquis → Gérer les bibliothèques → Chercher "Adafruit GFX"
 *
 * Fonctionnalités:
 * - Fond noir avec chiffres blancs en BELLE police vectorielle
 * - Mode Rouge activé par bouton GPIO 21
 * - Mode Vert en standby (pour usage futur)
 */

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "TCA9554.h"
#include "config.h"

// 🎨 POLICES ADAFRUIT GFX - Décommentez celle que vous préférez
#include <Fonts/FreeSansBold18pt7b.h>   // ✅ RECOMMANDÉ : gros et lisible
// #include <Fonts/FreeSansBold24pt7b.h>   // Encore plus grand !
// #include <Fonts/FreeMonoBold18pt7b.h>   // Style digital/rétro
// #include <Fonts/FreeSerifBold18pt7b.h>  // Style classique serif

// Police secondaire pour textes
#include <Fonts/FreeSans12pt7b.h>       // Texte élégant

// ============================================
// OBJETS GLOBAUX
// ============================================
TCA9554 TCA(TCA9554_ADDR);

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
  LCD_QSPI_CS, LCD_QSPI_CLK,
  LCD_QSPI_D0, LCD_QSPI_D1,
  LCD_QSPI_D2, LCD_QSPI_D3
);

Arduino_GFX *g = new Arduino_AXS15231B(
  bus, -1, 0, false,
  SCREEN_WIDTH, SCREEN_HEIGHT
);

Arduino_Canvas *gfx = new Arduino_Canvas(
  SCREEN_WIDTH, SCREEN_HEIGHT,
  g, 0, 0, ROTATION
);

// ============================================
// VARIABLES GLOBALES
// ============================================
int compteur = 0;
Mode modeActuel = MODE_STANDBY;
unsigned long dernierUpdate = 0;
unsigned long dernierDebounce = 0;
bool dernierEtatBouton = HIGH;

// ============================================
// PROTOTYPES
// ============================================
void initHardware();
void testAffichage();
void afficherCompteur(int valeur);
void afficherConsigne(String texte, uint16_t couleur = COLOR_WHITE);
void modeRouge();
void modeVert();
void gererBouton();

// ============================================
// SETUP
// ============================================
void setup(void) {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║  ESP32-S3 Compteur POLICES ADAFRUIT   ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  initHardware();
  testAffichage();

  // Écran initial : fond noir
  gfx->fillScreen(COLOR_BLACK);
  afficherConsigne("Pret - Appuyez BTN21", COLOR_CYAN);
  gfx->flush();

  Serial.println("\n✅ Setup terminé!");
  Serial.println("📌 Appuyez sur GPIO 21 pour Mode Rouge\n");
}

// ============================================
// INIT HARDWARE
// ============================================
void initHardware() {
  // I2C et TCA9554
  Wire.begin(I2C_SDA, I2C_SCL);
  TCA.begin();
  TCA.pinMode1(1, OUTPUT);
  TCA.write1(1, 1);
  delay(10);
  TCA.write1(1, 0);
  delay(10);
  TCA.write1(1, 1);
  delay(200);

  // Display
  if (!gfx->begin()) {
    Serial.println("❌ ERREUR: gfx->begin() failed!");
    while(1) { delay(1000); }
  }
  Serial.println("✓ Display initialisé");

  gfx->fillScreen(COLOR_BLACK);
  gfx->flush();

  // Backlight
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
  Serial.println("✓ Backlight ON");

  // Bouton
  pinMode(BTN_MODE_ROUGE, INPUT_PULLUP);
  Serial.println("✓ Bouton GPIO 21 configuré");
}

// ============================================
// TEST INITIAL
// ============================================
void testAffichage() {
  Serial.println("Test d'affichage...");

  const uint16_t couleurs[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE};

  for (int i = 0; i < 3; i++) {
    gfx->fillScreen(couleurs[i]);
    gfx->flush();
    delay(300);
  }

  gfx->fillScreen(COLOR_BLACK);

  // Test avec police Adafruit
  gfx->setFont(&FreeSansBold18pt7b);
  gfx->setTextColor(COLOR_YELLOW);
  gfx->setCursor(30, 100);
  gfx->println("ESP32-S3");
  gfx->setCursor(30, 150);
  gfx->println("Police OK!");
  gfx->flush();

  delay(1000);
}

// ============================================
// AFFICHER COMPTEUR (BELLE POLICE VECTORIELLE)
// ============================================
void afficherCompteur(int valeur) {
  // Effacer zone compteur
  gfx->fillRect(0, ZONE_COMPTEUR_Y, SCREEN_WIDTH, ZONE_COMPTEUR_H, COLOR_BLACK);

  // ✨ BELLE POLICE ADAFRUIT
  gfx->setFont(&FreeSansBold18pt7b);
  gfx->setTextColor(COLOR_WHITE);

  // Créer le texte
  String texte = String(valeur);

  // 🎯 Calculer position centrée (méthode précise)
  int16_t x1, y1;
  uint16_t w, h;
  gfx->getTextBounds(texte.c_str(), 0, 0, &x1, &y1, &w, &h);

  // Centrage horizontal (en tenant compte de x1 qui peut être négatif)
  int x = (SCREEN_WIDTH - w) / 2 - x1;

  // Position verticale (baseline des polices vectorielles)
  int y = ZONE_COMPTEUR_Y + 90;

  gfx->setCursor(x, y);
  gfx->print(texte);

  gfx->flush();
}

// ============================================
// AFFICHER CONSIGNE (police secondaire)
// ============================================
void afficherConsigne(String texte, uint16_t couleur) {
  // Effacer zone consigne
  gfx->fillRect(0, ZONE_CONSIGNE_Y, SCREEN_WIDTH, ZONE_CONSIGNE_H, COLOR_BLACK);

  // Police élégante pour consignes
  gfx->setFont(&FreeSans12pt7b);
  gfx->setTextColor(couleur);

  // Calculer position centrée
  int16_t x1, y1;
  uint16_t w, h;
  gfx->getTextBounds(texte.c_str(), 0, 0, &x1, &y1, &w, &h);

  int x = (SCREEN_WIDTH - w) / 2 - x1;
  int y = ZONE_CONSIGNE_Y + 30;

  gfx->setCursor(x, y);
  gfx->print(texte);

  gfx->flush();
}

// ============================================
// MODE ROUGE (activé par bouton)
// ============================================
void modeRouge() {
  static unsigned long dernierIncrement = 0;

  if (millis() - dernierIncrement >= INTERVALLE_COMPTEUR) {
    dernierIncrement = millis();

    compteur++;

    // Afficher le compteur
    afficherCompteur(compteur);

    // Consignes selon valeur
    if (compteur < 10) {
      afficherConsigne("Mode Rouge - Debut", COLOR_ORANGE);
    } else if (compteur < 50) {
      afficherConsigne("Mode Rouge - En cours", COLOR_RED);
    } else if (compteur >= 50) {
      afficherConsigne("Mode Rouge - ALERTE!", COLOR_RED);

      // Clignotement si > 50
      if (compteur % 2 == 0) {
        gfx->fillRect(0, 0, SCREEN_WIDTH, 50, COLOR_RED);
        gfx->flush();
      } else {
        gfx->fillRect(0, 0, SCREEN_WIDTH, 50, COLOR_BLACK);
        gfx->flush();
      }
    }

    Serial.print("🔴 Compteur Rouge: ");
    Serial.println(compteur);
  }
}

// ============================================
// MODE VERT (standby - pour usage futur)
// ============================================
void modeVert() {
  static unsigned long dernierIncrement = 0;

  if (millis() - dernierIncrement >= INTERVALLE_COMPTEUR) {
    dernierIncrement = millis();

    compteur++;

    // Afficher le compteur
    afficherCompteur(compteur);

    // Consignes
    if (compteur < 30) {
      afficherConsigne("Mode Vert - OK", COLOR_GREEN);
    } else {
      afficherConsigne("Mode Vert - Limite", COLOR_YELLOW);
    }

    Serial.print("🟢 Compteur Vert: ");
    Serial.println(compteur);
  }
}

// ============================================
// GERER BOUTON
// ============================================
void gererBouton() {
  bool lectureBouton = digitalRead(BTN_MODE_ROUGE);

  // Anti-rebond
  if (lectureBouton != dernierEtatBouton) {
    dernierDebounce = millis();
  }

  if ((millis() - dernierDebounce) > DEBOUNCE_DELAY) {
    if (lectureBouton == LOW && modeActuel != MODE_ROUGE) {
      // Bouton pressé et mode pas encore actif
      modeActuel = MODE_ROUGE;
      compteur = 0;  // Reset compteur

      // Feedback visuel
      gfx->fillScreen(COLOR_BLACK);
      afficherConsigne("MODE ROUGE DEMARRE!", COLOR_RED);
      gfx->flush();
      delay(500);

      Serial.println("\n🔴 ===== MODE ROUGE DÉMARRÉ =====\n");
    }
  }

  dernierEtatBouton = lectureBouton;
}

// ============================================
// LOOP
// ============================================
void loop() {
  // Vérifier le bouton
  gererBouton();

  // Exécuter le mode actif
  switch (modeActuel) {
    case MODE_ROUGE:
      modeRouge();
      break;

    case MODE_VERT:
      // Fonction prête mais non utilisée pour l'instant
      modeVert();
      break;

    case MODE_STANDBY:
      // Rien à faire en standby
      break;
  }

  delay(10);
}
