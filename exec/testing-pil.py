#!/usr/bin/python

# INFORMACION DEL SCRIPT
# ---------------------------------------------------------
# AUTOR   : Ezequiel Larrarte
# FECHA   : 28-03-2020
# VERSION : 01
#
# DESCRIPCION
# Este script lee las las imagenes (cartas) de un directorio
# y las agrupa en un PDF con el objetivo de imprimirlas.
# El tamaño de la hoja y de la carta se pueden especificar.
# ---------------------------------------------------------

# LIBRERIAS
# ---------------------------------------------------------
import os
import sys
if "SCRIPTS" in os.environ.keys():
    sys.path.append(os.environ['SCRIPTS'] + '/include')
from posix_exitcodes import *
import main
import glob
from PIL import Image
# ---------------------------------------------------------

# CONFIGURACION
# ---------------------------------------------------------
d_pages = {"a4": {"width": 210, "height": 297}}
d_cards = {"usa": {"width": 56, "height": 87}, "ocean": {"width": 63.5, "height": 88}}
n_card_pad = 50
img_page = d_pages["a4"]
img_card = d_cards["ocean"]
n_img_dpi = 300
t_card_path = glob.escape("/srv/Downloads/Aeons End PnP [EN]/Ayudas y Turnos")
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------
def mm_to_px(mm, dpi):
    return (mm * dpi) / 25.4

def process_card(t_card_file):
    global n_count_row, n_count_col
    global n_offset_w, n_offset_h
    global n_offset_base_w, n_offset_base_h
    global n_cards_per_row, n_cards_per_col
    global n_card_w, n_card_h
    global img, l_img

    if n_count_row < (n_cards_per_row - 1):
        n_count_row += 1
        n_offset_w += n_card_w + n_card_pad
    else:
        if n_count_col < (n_cards_per_col - 1):
            n_count_row = 0
            n_count_col += 1
            n_offset_w = n_offset_base_w
            n_offset_h += n_card_h + n_card_pad
        else:
            n_count_row = 0
            n_count_col = 0
            n_offset_w = n_offset_base_w
            n_offset_h = n_offset_base_h
            img = Image.new("RGB", (n_page_w, n_page_h), (255, 255, 255))
            l_img += [img]

    card = Image.open(t_card_file)
    card.resize((n_card_w, n_card_h))
    img.paste(card, box=(n_offset_w, n_offset_h))
    card.close()
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------
n_page_w = int(mm_to_px(img_page["width"], n_img_dpi))
n_page_h = int(mm_to_px(img_page["height"], n_img_dpi))
n_card_w = int(mm_to_px(img_card["width"], n_img_dpi))
n_card_h = int(mm_to_px(img_card["height"], n_img_dpi))

n_cards_per_row = n_page_w // n_card_w
n_cards_per_col = n_page_h // n_card_h
n_offset_base_w = (n_page_w - n_cards_per_row*n_card_w - (n_cards_per_row-1)*n_card_pad) // 2
n_offset_base_h = (n_page_h - n_cards_per_col*n_card_h - (n_cards_per_col-1)*n_card_pad) // 2

n_count_row = n_cards_per_row
n_count_col = n_cards_per_col

l_img = []

for t_card_file in glob.glob(t_card_path + "/*.jpg"):
    n_repetition = int(t_card_file.split(".")[0].split(" ")[-1:][0][1:])
    for i in range(n_repetition):
        print(t_card_file)
        process_card(t_card_file)

l_img[0].save("img.pdf", save_all=True, append_images=l_img[1:])

exit(EXIT_SUCCESS)
# ---------------------------------------------------------


