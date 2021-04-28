#!/usr/bin/python3

from PIL import Image, ImageDraw
import argparse
import re

# set the argument parser
argparser = argparse.ArgumentParser(description="Welcome to cropper, a PIL based card croppper for PnP games!")
argparser.add_argument("--image-file", dest="image_file", type=str, help="image file for cropping", required=True)
argparser.add_argument("--x-start", dest="x_start", type=int, help="the start pixel for cropping in the X axis", required=True)
argparser.add_argument("--y-start", dest="y_start", type=int, help="the start pixel for cropping in the Y axis", required=True)
argparser.add_argument("--x-card-count", dest="x_card_count", type=int, help="the number of cards to crop in the X axis", required=True)
argparser.add_argument("--y-card-count", dest="y_card_count", type=int, help="the number of cards to crop in the Y axis", required=True)
argparser.add_argument("--x-card-size", dest="x_card_size", type=int, help="the number of pixels the card has in the X axis", required=True)
argparser.add_argument("--y-card-size", dest="y_card_size", type=int, help="the number of pixels the card has in the Y axis", required=True)
argparser.add_argument("--x-separation", dest="x_separation", type=int, help="the number of pixels between cards in the X axis", required=True)
argparser.add_argument("--y-separation", dest="y_separation", type=int, help="the number of pixels between cards in the Y axis", required=True)
argparser.add_argument("--card-count", dest="card_count", type=int, help="the card count initial number", default=1)
argparser.add_argument("--card-padding", dest="card_padding", type=int, help="the card filename padding", default=0)
argparser.add_argument("--card-repetition", dest="card_repetition", type=int, help="the repetition for each card on this set", default=1)
args = argparser.parse_args()

# extract variables from argument parser
image_file = args.image_file
x_card_count = args.x_card_count
y_card_count = args.y_card_count
x_start = args.x_start
y_start = args.y_start
x_card_size = args.x_card_size
y_card_size = args.y_card_size
x_separation = args.x_separation
y_separation = args.y_separation
card_count = args.card_count
card_repetition = args.card_repetition
if args.card_repetition == 1:
	regex_result = re.search("\sx(\d+)\.()", image_file)
	if type(regex_result) == re.Match:
		card_repetition = int(regex_result.group(1))
if args.card_padding != 0:
	card_padding = args.card_padding
else:
	card_padding = max(2, len(str(x_card_count * y_card_count)))

# open image file
image_handler = Image.open(image_file)

# cropping loop
for x in range(x_card_count):
	for y in range(y_card_count):
		# set card boundaries
		card_x_start = x_start + (x_separation * x) + (x_card_size * x)
		card_y_start = y_start + (y_separation * y) + (y_card_size * y)
		card_x_end = card_x_start + x_card_size
		card_y_end = card_y_start + y_card_size

		# set card file name
		if card_repetition > 1:
			card_file = "%s x%d.jpg" % (str(card_count).rjust(card_padding, "0"), card_repetition)
		else:
			card_file = "%s.jpg" % (str(card_count).rjust(card_padding, "0"))

		# print card cropping information
		print("card [%s] => [%4d, %4d, %4d, %4d]" % (card_file, card_x_start, card_y_start, card_x_end, card_y_end))

		# crop card
		card = image_handler.crop((card_x_start, card_y_start, card_x_end, card_y_end))
		
		# draw a black rectangle in the perimeter of the card
		card_draw = ImageDraw.Draw(card)
		card_draw.rectangle([(0, 0), card.size], outline=(0, 0, 0), width=4)

		# create the image file
		card.save(card_file, "JPEG")

		# increment card count
		card_count += 1

# close image file
image_handler.close()

