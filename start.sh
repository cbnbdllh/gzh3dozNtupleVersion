# ! /bin/bash
./gzh3doz gun.mac
mkdir results
cp gzh3doz.root 500.root
mv 500.root results/
sed -i -e 's/500 keV/1000 keV/g' gun.mac
./gzh3doz gun.mac
cp gzh3doz.root 1000.root
mv 1000.root results/
sed -i -e 's/1000 keV/4000 keV/g' gun.mac
./gzh3doz gun.mac
cp gzh3doz.root 4000.root
mv 4000.root results/
sed -i -e 's/4000 keV/8000 keV/g' gun.mac
./gzh3doz gun.mac
cp gzh3doz.root 8000.root
mv 8000.root results/
sed -i -e 's/8000 keV/12000 keV/g' gun.mac
./gzh3doz gun.mac
cp gzh3doz.root 12000.root
mv 12000.root results/
sed -i -e 's/12000 keV/16000 keV/g' gun.mac
./gzh3doz gun.mac
cp gzh3doz.root 16000.root
mv 16000.root results/
sed -i -e 's/16000 keV/20000 keV/g' gun.mac
./gzh3doz gun.mac
cp gzh3doz.root 20000.root
mv 20000.root results/

