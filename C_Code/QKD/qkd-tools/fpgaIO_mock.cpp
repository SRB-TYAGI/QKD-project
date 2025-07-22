namespace fpgaIO {
    void* FPGA_Open() {
        return nullptr;  // simulate hardware handle
    }

    void FPGA_Close(void* handle) {
        // do nothing
    }

    void FPGA_Interface(void* handle, unsigned char* inBuf, unsigned char* outBuf, int& size) {
        // fake some response: just zero out output or copy input to output
        for (int i = 0; i < size; ++i) {
            outBuf[i] = 0;  // or inBuf[i] if passthrough behavior is better
        }
    }
}
