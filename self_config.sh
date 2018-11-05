make distclean 2>&1 > /dev/null
rm -rf $(pwd)/objs/

./configure --prefix=$(pwd)/objs/

mkdir -p $(pwd)/objs/man/man1/

#sed -i 's/gcc/powerpc-linux-gcc/g' $(find ./ -name Makefile)


mkdir -p $(pwd)/objs/etc/
cat > $(pwd)/objs/etc/thttpd.conf <<-EOF
# This section overrides defaults
dir=$(pwd)/objs/www
#chroot
#user=httpd# default = nobody
#logfile=$(pwd)/objs/thttpd.log
cgipat=/cgi-bin/**
#pidfile=$(pwd)/objs/thttpd.pid
# This section _documents_ defaults in effect
port=80
# nosymlink# default = !chroot
# novhost
# nocgipat
# nothrottles
# host=0.0.0.0
charset=UTF-8
EOF
