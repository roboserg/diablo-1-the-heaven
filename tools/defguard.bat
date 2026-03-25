awk --assign=name=%2 --file=%awk% %1 > temp.tmp
del %1
ren temp.tmp %1
