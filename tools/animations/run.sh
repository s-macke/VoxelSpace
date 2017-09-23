set -e

mkdir -p images

echo delete
rm -f images/*.png
rm -f images/*.gif

echo anim
python anim.py

echo convert
#convert images/out*.png -layers OptimizeFrame -delay 100 -loop 0 out.gif
#gifsicle out.gif > outoptim.gif

gifsicle --colors 256 --optimize=2 --delay=10 --loop images/*.gif > anim.gif
