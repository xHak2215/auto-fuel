void write_number_to_eerom(int address, short number) {
    EEPROM.write(address,        (uint8_t)(number & 0xFF));        // low byte
    EEPROM.write(address + 1,    (uint8_t)((number >> 8) & 0xFF)); // high byte
    EEPROM.commit();
}

short read_number_in_eerom(int address) {
    uint8_t low  = EEPROM.read(address);
    uint8_t high = EEPROM.read(address + 1);

    return (short)((high << 8) | low);
}
