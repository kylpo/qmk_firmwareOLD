MOUSEKEY_ENABLE = yes       # Mouse keys
TAP_DANCE_ENABLE = no
COMBO_ENABLE = yes

RGBLIGHT_ENABLE = yes        # Enable keyboard RGB underglow
OLED_DRIVER_ENABLE = yes
LTO_ENABLE = yes

CONSOLE_ENABLE = yes

# If you want to change the display of OLED, you need to change here
SRC +=  ./common/oled_helper.c \
