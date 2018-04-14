CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
SET(CMAKE_SYSTEM_NAME Generic)

#============================================================================================
PROJECT(AVR)

SET(DEVICE "atmega256RFR2")
# see list https://www.nongnu.org/avrdude/user-manual/avrdude_4.html
set(CHIPSET m256rfr2) # m256rfr2
SET(FREQ "1000000")
set(PROGRAMMER xplainedpro)

SET(FLASH false)

# will clean flash memory of device (used for reseting)
SET(CLEAN_DEVICE false)


#============================================================================================
# fix mac aquires device
# https://github.com/jakeh12/atmel-ice-mac-driver

# buy starter Kit (currently 117 EURO)
# https://www.element14.com/community/docs/DOC-55010/l/xplained-pro-starter-kit-for-atmega256rfr2-microcontrollers


# find usb device https://de.mathworks.com/help/supportpkg/arduinoio/ug/find-arduino-port-on-windows-mac-and-linux.html

set(USB usb)


#============================================================================================

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CMAKE_C_FLAGS "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -std=gnu99 -Wl,--gc-sections")
SET(CMAKE_CXX_FLAGS "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -Wl,--gc-sections")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

#============================================================================================

SET(SOURCE_FILES

        main.c

        )

#============================================================================================

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

if (CLEAN_DEVICE)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avrdude -e -b 38400 -p ${CHIPSET} -P ${USB} -c ${PROGRAMMER})

    return(0)
endif ()



ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avr-objcopy -O ihex -R.eeprom ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.eep)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avr-size ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} --mcu=${DEVICE} --totals --format=avr)




if (FLASH)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avrdude -b 38400 -p ${CHIPSET} -P ${USB} -c ${PROGRAMMER} -U flash:w:../bin/${PROJECT_NAME}.hex)
else ()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avrdude -D -b 38400 -p ${CHIPSET} -P ${USB} -c ${PROGRAMMER} -U flash:r:../bin/${PROJECT_NAME}.dump:r)
endif ()
