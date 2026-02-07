#include "churmant.h"

func recover() do
  println("unable to install the compiler to your system");
  exit(failure);
end

churmant_main
  if churmant_os == CHURMANT_WINDOWS then
    system("powershell \"wget https://github.com/antagonistry/Churmant/archive/master.zip -OutFile Churmant.zip\"");
    system("powershell \"Expand-Archive Churmant.zip -DestinationPath Churmant -Force -ErrorAction SilentlyContinue\"");
    system("powershell \"Move-Item Churmant\\Churmant-master %USERPROFILE%\\.Churmant\" -Force -ErrorAction SilentlyContinue\"");
    system("powershell \"Remove-Item Churmant.zip -Force -ErrorAction SilentlyContinue\"");
    system("powershell \"Remove-Item Churmant -Force -ErrorAction SilentlyContinue\"");
  else
    system("curl https://github.com/antagonistry/Churmant/archive/master.zip -L -o Churmant.zip");
    system("unzip Churmant.zip -d Churmant");
    system("mv Churmant/Churmant-master ~/.Churmant");
    system("rm -rf Churmant.zip");
    system("rm -rf Churmant");
  end
  println("successfully cloned the repository to home folder");
churmant_mend(recover)
