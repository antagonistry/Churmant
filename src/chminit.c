func(clone())
  tiny ret = 0;
  println("(churmant/installer) cloning 'Churmant' repository to '~/.Churmant'");
  
  if churmant_os == CHURMANT_WINDOWS then
    system("powershell \"wget https://github.com/antagonistry/Churmant/archive/master.zip -OutFile Churmant.zip\"");
    system("powershell \"Expand-Archive Churmant.zip -DestinationPath Churmant -Force -ErrorAction SilentlyContinue\"");
    ret = system("powershell \"Move-Item Churmant\\Churmant-master %USERPROFILE%\\.Churmant\" -Force -ErrorAction SilentlyContinue\"");
    system("powershell \"Remove-Item Churmant.zip -Force -ErrorAction SilentlyContinue\"");
    system("powershell \"Remove-Item Churmant -Force -ErrorAction SilentlyContinue\"");
  else
    system("curl https://github.com/antagonistry/Churmant/archive/master.zip -L -o Churmant.zip");
    system("unzip Churmant.zip -d Churmant");
    ret = system("mv Churmant/Churmant-master ~/.Churmant");
    system("rm -rf Churmant.zip");
    system("rm -rf Churmant");
    system("read -p \"press any key to continue\"");
  end

  if ret == 0 then
    println("(churmant/installer) successfully cloned 'Churmant' repository to '~/.Churmant'");
  else
    println("(churmant/installer) unable to clone 'Churmant' repository to '~/.Churmant'");
    exit(failure);
  end
fend(abort)

func(compile())
  tiny ret = 0;
  println("(churmant/installer) compiling 'chmc' compiler");
  if churmant_os == CHURMANT_WINDOWS then
    ret = system("gcc -O3 -ffast-math -march=native -g -I %USERPROFILE%\\.Churmant\\include -Werror=uninitialized -o %USERPROFILE%\\.Churmant\\bin\\chmc %USERPROFILE%\\.Churmant\\src\\chmc.c\n");
  else
    ret = system("gcc -O3 -ffast-math -march=native -g -I ~/.Churmant/include -Werror=uninitialized -o ~/.Churmant/bin/chmc ~/.Churmant/src/chmc.c\n");
  end
  if ret == 0 then
    println("(churmant/installer) successfully compiled 'chmc' compiler");
  end
  if ret == 1 then
    println("(churmant/installer) failed to compile 'chmc' compiler");
    exit(failure);
  end
  if ret == 127 then
    println("(churmant/installer) 'gcc' compiler is not installed");
    exit(failure);
  end
fend(abort)

churmant_main
  final int CHOICE_SIZE = size(char) * 4;
  string choice = null;
  allocate(choice, CHOICE_SIZE);
  print("(churmant/installer) clone 'Churmant' repository [Y/n] ");
  file_readline(choice, CHOICE_SIZE, stdin);
  
  match(*choice)
    case('y')
      clone();
    close
    case('Y')
      clone();
    close
    case('n')
      break;
    close
    case('N')
      break;
    close
    default
      println("(churmant/installer) the choice you've entered is invalid");
      
      if churmant_os == CHURMANT_WINDOWS then
        system("pause");
      end

      exit(failure);
    close
  end 

  print("(churmant/installer) compile the compiler [Y/n] ");
  file_readline(choice, CHOICE_SIZE, stdin);
  
  match(*choice)
    case('y')
      compile();
    close
    case('Y')
      compile();
    close
    case('n')
      break;
    close
    case('N')
      break;
    close
    default
      println("the choice you've entered is invalid");
      exit(failure);
    close
  end

  if churmant_os == CHURMANT_WINDOWS then
    system("pause");
  end
churmant_mend
