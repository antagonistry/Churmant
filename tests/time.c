churmant_main
  final int CMD_SIZE = sizeof(char) * 256;
  struct timeval start_time, end_time;
  float elapsed = 0.0f;
  string cmd = null;
  allocate(cmd, CMD_SIZE);
  strncpy(cmd, "", sizeof(cmd));
  for(1, i < argc, 1) do
    strncat(cmd, argv[i], CMD_SIZE);
    if i + 1 < argc then
      strncat(cmd, " ", CMD_SIZE);
    end
  end
  match(*cmd)
    case('\0')
      gettimeofday(&start_time, NULL);
      gettimeofday(&end_time, NULL);
    close
    default
      gettimeofday(&start_time, NULL);
      system(cmd);
      gettimeofday(&end_time, NULL);
    close
  end
  elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000;
  elapsed += (end_time.tv_usec - start_time.tv_usec) / 1000;
  printf("%-16s%lf\n", "elapsed time", elapsed);
churmant_mend
