set -e

mkdir -p images

echo delete
rm -f images/*.png
rm -f images/*.gif

echo anim
python anim.py

echo convert
gifsicle --optimize=3 --scale=0.5 --delay=5 --loop images/*.gif > anim.gif
