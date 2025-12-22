# CLI Programını Executable Yapma Kılavuzu

## İçindekiler
1. [PATH'e Ekleme (Global Erişim)](#1-pathe-ekleme-global-erişim)
2. [Install Script Yazma](#2-install-script-yazma)
3. [Desktop Entry Oluşturma (Ubuntu/Linux)](#3-desktop-entry-oluşturma-ubuntulinux)
4. [Mac'te .app Bundle Oluşturma](#4-macte-app-bundle-oluşturma)
5. [Static Linking (Bağımlılıkları Dahil Etme)](#5-static-linking-bağımlılıkları-dahil-etme)
6. [Package Oluşturma](#6-package-oluşturma)
7. [Uninstall Script](#7-uninstall-script)
8. [Makefile'a Install Hedefi Ekle](#8-makefileа-install-hedefi-ekle)
9. [Özet Karşılaştırma](#özet-karşılaştırma)

---

## 1. PATH'e Ekleme (Global Erişim)

### Mantık:
System-wide veya user-specific PATH dizinine binary'yi kopyala/link et.

### Ubuntu/Linux:

#### Yöntem A: /usr/local/bin (Önerilen - Tüm kullanıcılar)
```bash
# Binary'yi kopyala
sudo cp minishell /usr/local/bin/

# Veya symbolic link oluştur (geliştirirken pratik)
sudo ln -s /tam/path/to/minishell /usr/local/bin/minishell

# Artık her yerden çalıştırabilirsin
minishell
```

#### Yöntem B: ~/.local/bin (Sadece senin için)
```bash
# Klasör yoksa oluştur
mkdir -p ~/.local/bin

# Link veya kopyala
ln -s /tam/path/to/minishell ~/.local/bin/minishell

# ~/.bashrc veya ~/.zshrc'ye ekle (yoksa):
export PATH="$HOME/.local/bin:$PATH"

# Reload
source ~/.bashrc
```

### Mac:
```bash
# Aynı şekilde /usr/local/bin kullan
sudo cp minishell /usr/local/bin/

# Veya Homebrew tarzı
sudo cp minishell /opt/homebrew/bin/  # M1/M2 Mac
sudo cp minishell /usr/local/bin/     # Intel Mac
```

---

## 2. Install Script Yazma

### Mantık:
Kullanıcılar için kolay kurulum scripti. Binary'yi doğru yere kopyalar, gerekli ayarları yapar.

### install.sh Örneği:
```bash
#!/bin/bash

# Renkler
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Binary adı
BINARY="minishell"
INSTALL_DIR="/usr/local/bin"

echo "🐚 Installing Minishell..."

# Binary var mı kontrol et
if [ ! -f "./$BINARY" ]; then
    echo "${RED}Error: $BINARY not found. Run 'make' first.${NC}"
    exit 1
fi

# Sudo gerekiyor mu?
if [ "$EUID" -ne 0 ]; then
    echo "Installing to $INSTALL_DIR requires sudo..."
    sudo cp "./$BINARY" "$INSTALL_DIR/$BINARY"
else
    cp "./$BINARY" "$INSTALL_DIR/$BINARY"
fi

# Execute permission ver
sudo chmod +x "$INSTALL_DIR/$BINARY"

echo "${GREEN}✓ Minishell installed successfully!${NC}"
echo "Run: $BINARY"
```

### Kullanımı:
```bash
make
sudo ./install.sh
```

---

## 3. Desktop Entry Oluşturma (Ubuntu/Linux)

### Mantık:
`.desktop` dosyası oluştur → Applications menüsünde görünür → Çift tıklayarak açılır.

### Yapısı:

**Dosya:** `/usr/share/applications/minishell.desktop` (system-wide)
**Veya:** `~/.local/share/applications/minishell.desktop` (user-only)

### İçerik:
```ini
[Desktop Entry]
Version=1.0
Type=Application
Name=Minishell
Comment=A bash-like shell implementation
Exec=gnome-terminal -- /usr/local/bin/minishell
Icon=utilities-terminal
Terminal=true
Categories=System;TerminalEmulator;ConsoleOnly;
Keywords=shell;bash;terminal;
StartupNotify=true
```

### Açıklama:
- `Exec`: Çalıştırılacak komut (terminal emulator içinde aç)
- `Icon`: Gösterilecek ikon
- `Terminal=true`: Terminal gerektiğini belirt
- `Categories`: Menüde nerede görüneceği

### Icon eklemek:
```bash
# Icon dosyası koy
sudo cp minishell.png /usr/share/pixmaps/minishell.png

# Desktop file'da:
Icon=/usr/share/pixmaps/minishell.png
```

### Otomatik kurulum için install.sh'a ekle:
```bash
# Desktop entry oluştur
cat > ~/.local/share/applications/minishell.desktop << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Minishell
Exec=gnome-terminal -- $INSTALL_DIR/$BINARY
Icon=utilities-terminal
Terminal=true
Categories=System;TerminalEmulator;
EOF

# Desktop entry'yi güncelle
update-desktop-database ~/.local/share/applications/
```

---

## 4. Mac'te .app Bundle Oluşturma

### Mantık:
Mac'te uygulamalar `.app` bundle'ı (aslında klasör). Çift tıklayınca açılır.

### Yapısı:
```
Minishell.app/
└── Contents/
    ├── Info.plist          # Metadata
    ├── MacOS/
    │   └── minishell       # Binary
    └── Resources/
        └── icon.icns       # Icon (optional)
```

### Info.plist:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
  "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>minishell</string>
    <key>CFBundleName</key>
    <string>Minishell</string>
    <key>CFBundleIdentifier</key>
    <string>com.yourname.minishell</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>LSUIElement</key>
    <false/>
</dict>
</plist>
```

### Otomatik oluşturma scripti:
```bash
#!/bin/bash

APP_NAME="Minishell"
BUNDLE_DIR="$APP_NAME.app/Contents/MacOS"

# Bundle yapısını oluştur
mkdir -p "$BUNDLE_DIR"

# Binary'yi kopyala
cp minishell "$BUNDLE_DIR/"

# Info.plist oluştur
cat > "$APP_NAME.app/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" ...>
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>minishell</string>
    ...
</dict>
</plist>
EOF

echo "✓ $APP_NAME.app created!"
echo "Drag to /Applications folder"
```

### Dikkat:
Terminal içinde çalışması için wrapper script gerekir.

**MacOS/minishell (wrapper):**
```bash
#!/bin/bash
# Terminal aç ve minishell'i çalıştır
osascript -e 'tell application "Terminal" to do script "'"$(dirname "$0")/minishell-bin"'"'
```

---

## 5. Static Linking (Bağımlılıkları Dahil Etme)

### Mantık:
Normalde dynamic linking kullanılır (readline libini sistemden alır). Static linking ile tüm kütüphaneler binary'ye gömülür → portable olur.

### Makefile'da:
```makefile
# Dynamic (default)
LDFLAGS = -lreadline -lncurses

# Static linking
LDFLAGS = -static -lreadline -lncurses -ltinfo
```

### Avantaj/Dezavantaj:
- **Avantajı:** Başka sistemde readline olmasa bile çalışır
- **Dezavantajı:** Binary boyutu büyür (~5-10 MB)

**Not:** Mac'te tam static linking zor (Apple öneride bulunmaz), Linux'ta daha kolay.

---

## 6. Package Oluşturma

### Ubuntu (.deb package):

#### Yapı:
```
minishell-deb/
├── DEBIAN/
│   └── control         # Package metadata
└── usr/
    └── local/
        └── bin/
            └── minishell
```

#### control dosyası:
```
Package: minishell
Version: 1.0
Architecture: amd64
Maintainer: Senin Adın <email@example.com>
Description: A bash-like shell implementation
Depends: libreadline8, libncurses5
```

#### Build:
```bash
dpkg-deb --build minishell-deb
```

#### Kurulum:
```bash
sudo dpkg -i minishell-deb.deb
```

### Mac (Homebrew Formula):

#### Formula örneği:
```ruby
class Minishell < Formula
  desc "A bash-like shell implementation"
  homepage "https://github.com/yourname/minishell"
  url "https://github.com/yourname/minishell/archive/v1.0.tar.gz"
  sha256 "..."

  depends_on "readline"

  def install
    system "make"
    bin.install "minishell"
  end

  test do
    system "#{bin}/minishell", "--version"
  end
end
```

#### Kullanıcılar için:
```bash
brew install minishell
```

---

## 7. Uninstall Script

```bash
#!/bin/bash

BINARY="minishell"
INSTALL_DIR="/usr/local/bin"

echo "Removing $BINARY..."

# Binary'yi sil
sudo rm -f "$INSTALL_DIR/$BINARY"

# Desktop entry'yi sil
rm -f ~/.local/share/applications/minishell.desktop

# Cache'i güncelle
update-desktop-database ~/.local/share/applications/ 2>/dev/null

echo "✓ Uninstalled successfully"
```

---

## 8. Makefile'a Install Hedefi Ekle

```makefile
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

install: $(NAME)
	@echo "Installing $(NAME) to $(BINDIR)..."
	@install -d $(BINDIR)
	@install -m 755 $(NAME) $(BINDIR)/$(NAME)
	@echo "✓ Installed successfully"

uninstall:
	@echo "Removing $(NAME) from $(BINDIR)..."
	@rm -f $(BINDIR)/$(NAME)
	@echo "✓ Uninstalled successfully"
```

### Kullanımı:
```bash
make
sudo make install

# Kaldırmak için
sudo make uninstall
```

---

## Özet Karşılaştırma

| Yöntem | Kullanım | Avantaj | Dezavantaj |
|--------|----------|---------|------------|
| **PATH'e ekle** | `cp /usr/local/bin` | Basit, hızlı | Manuel işlem |
| **Install script** | `./install.sh` | Otomatik, kullanıcı dostu | Script yazmak gerek |
| **Desktop entry** | GUI'den çift tıkla | Görsel, kolay | Sadece Linux |
| **.app bundle** | Finder'dan çift tıkla | Mac-native | Kompleks yapı |
| **Static link** | Portable binary | Bağımlılık yok | Büyük dosya |
| **.deb package** | `apt install` | Profesyonel | Paket bilgisi gerek |
| **Homebrew** | `brew install` | Mac standard | Formula yazmalısın |

---

## Önerilen Kombinasyon

Projen için ideal yaklaşım:

1. **Makefile'a install/uninstall hedefi ekle** → Kolay kurulum
2. **Install script yaz** → Otomasyonlu
3. **Desktop entry oluştur (Linux)** → GUI erişim
4. **İleride .deb ve Homebrew** → Distribution

---

## Notlar

- CLI programları mutlaka terminal emulator içinde çalışmalı (readline dependency)
- Static linking portable yapar ama dosya boyutunu artırır
- Desktop entry ve .app bundle sadece GUI erişim için, program gene terminal açar
- Production için package management sistemleri (.deb, Homebrew) profesyonel yaklaşım

---

**Tarih:** 2025-12-22
**Proje:** Minishell
**Amaç:** CLI programını sistem genelinde kullanılabilir hale getirme yöntemleri
