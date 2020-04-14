mkdir libs_build
cd libs_build


echo "Download dependencies"
Invoke-WebRequest https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.zip -OutFile boost.zip
Invoke-WebRequest https://slproweb.com/download/Win64OpenSSL-1_1_1f.exe -OutFile openssl.exe

echo "Install OpenSSL"
Start-Process ./openssl.exe /verysilent -Wait

mkdir openssl
cp "C:/Program Files/OpenSSL-Win64/libcrypto-1_1-x64.dll" ./openssl/
cp "C:/Program Files/OpenSSL-Win64/libssl-1_1-x64.dll" ./openssl/

echo "Unpack Boost"
Expand-Archive boost.zip -Force -DestinationPath boost

echo "Bootstrap Boost"
cd ./boost/boost_1_71_0
./bootstrap.bat

echo "Install Boost"
./b2 install
cd ../../

echo "Delete temp files"
rm boost.zip
rm openssl.exe
