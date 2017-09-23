set -e

mkdir -p images

echo delete
rm -f images/*.png
rm -f images/*.gif

echo anim
python drawmap.py

echo convert

gifsicle --colors 256 --optimize=2 --delay=200 --loop images/*.gif > periodicmap.gif
