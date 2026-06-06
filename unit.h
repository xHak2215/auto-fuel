char* write_number_to_eerom(int address, int number){     // переделать на нармальне хранение по типу short 
    char r_bin[12] = {0}, bin[13] = {0};
    short temp, i = 0;
    
    temp = number / 2;
    if (number % 2 > 0)
        r_bin[i] = '1';
    else
        r_bin[i] = '0';

    while(temp != 0) {
        i++;
        if (temp % 2 > 0)
            r_bin[i] = '1';
        else    
            r_bin[i] = '0';

        temp = temp / 2;
    }

    short len = strlen(r_bin);                                              
    short idx = 0;
    for (short j = len - 1; j >= 0; j--) {
        EEPROM.write(address++, (byte)r_bin[j]);
        bin[idx] = r_bin[j];
        idx++;
    }
    bin[idx++] = '\0';
    //EEPROM.write(address+((idx+1)), (byte)255);
    EEPROM.commit();   // Сохранение изменений
    return bin;
}

char* read_number_in_eerom(int address) {
    int temp, i=0;
    char bin[13];

    while (temp != 255){
        temp = (int)EEPROM.read(address + i);
        bin[i] = temp;
        i++;
    }
    return bin;
}
