#include "churmant.h"

func recover() do
  println("unable to install the compiler to your system");
  exit(failure);
end

churmant_main
  if churmant_os == CHURMANT_WINDOWS then
    system("wget https://github.com/antagonistry/Churmant/archive/master.zip -OutFile Churmant.zip");
    system("unzip Churmant.zip -d ~");
    system("powershell \"mv ~/Churmant-master ~/.Churmant\"");
    system("start %USERPROFILE%\\.Churmant\\build.bat");
    _putenv("%USERPROFILE%\\Churmant\\bin");
  end
churmant_mend(recover)
