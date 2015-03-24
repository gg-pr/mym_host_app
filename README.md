
Steps to build the code

-DUNICODE is required to get over wchar type issues
-static-libgcc & -static-libstdc++ is needed to get over loading issues related to DLL libgcc_s_dw2-1.dll.
-lpthreadGC2 is a static pthread library built locally

g++ -o host_main host_main.cpp mymclient.cpp mymserver.cpp -D
UNICODE -Bstatic -lWs2_32 -static-libgcc -static-libstdc++ -Bstatic -lpthreadGC2 -lpthread