cmake_minimum_required(VERSION 3.13.1)

if (CONFIG_AUTO_CORRECT_MODULE)
  target_sources(app PRIVATE auto_correct.c)
endif()
