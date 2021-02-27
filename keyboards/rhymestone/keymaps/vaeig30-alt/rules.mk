MOUSEKEY_ENABLE = yes
COMBO_ENABLE = yes

# Increase compile time, but reduce compiled size on firmware
LTO_ENABLE = yes

# Needed to monitor Capslock status
# See https://github.com/qmk/qmk_firmware/issues/2471#issuecomment-452271130
KEYBOARD_SHARED_EP = yes 

# Needed for media keys
EXTRAKEY_ENABLE = yes

# CONSOLE_ENABLE = yes