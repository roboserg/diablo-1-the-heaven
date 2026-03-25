@rem forfiles /P исходный_путь /M маска_файлов /S /С "команда_для_файла"
set awk=%2
forfiles /P %1 /M *.h /S /C "cmd /c defguard.bat @file @fname"

