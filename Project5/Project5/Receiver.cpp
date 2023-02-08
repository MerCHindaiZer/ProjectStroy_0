#include "Receiver.h"
// Receiver.cpp
HANDLE hSerial;
char ReadCOM()
{
    DWORD iSize;
    char sReceivedChar;
    string param;

    ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
    if (iSize >= 0)   // если что-то принято, выводим

        cout << sReceivedChar;
        return sReceivedChar;
}

float COM(int& adr, float& rotate_X, float& rotate_Y, float& rotate_Z) {

    LPCTSTR sPortName = L"COM3";
    hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {

        }

    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {

    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams))
    {

    }

    char data[] = " ";  // строка для передачи
    DWORD dwSize = sizeof(data);   // размер этой строки
    DWORD dwBytesWritten;    // тут будет количество собственно переданных байт

    BOOL iRet = WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);
    int iter = 0; // = 0
    while (1) {
        string buf, buf2;

        char omeba[25]; // '|' = 124

        if (iter <= 1) {

            for (int i = 0; i < 25; i++) {
                omeba[i] = ReadCOM();
                buf2 += omeba[i];
                //cout << buf2;
                if (omeba[i] == 124) {

                    buf = buf2;
                    iter++;

                    break;

                }
            }
        }
        else {
            for (int i = 0; i < 25; i++) {
                omeba[i] = ReadCOM();
                buf2 += omeba[i];

                if (omeba[i] == 124) {

                    buf = buf2;
                    break;
                }
            }


            string datastr;

            for (int i = 0; i < 3; i++) {
                datastr = buf.substr(0, buf.find("|"));
                // cout << datastr;  

                string type = datastr.substr(0, 4);
                //cout << type;
                /*
                if (type == "\r\nD:") {
                    //cout << "find D data" << endl;
                    string strtoD;
                    strtoD = datastr.substr(4, datastr.find("|"));
                    dt = stof(strtoD);
                    cout << "D= "<< dt << endl;

                }
                */
                if (type == "\r\nA:") {
                    //cout << "find address data" << endl;
                    string address;
                    address = datastr.substr(4, datastr.find("|"));
                    adr = stof(address);
                    //cout << "A= " << XX << endl;

                }
                if (type == "\r\nX:") {
                    //cout << "find X data" << endl;
                    string strtoX;
                    strtoX = datastr.substr(4, datastr.find("|"));
                    rotate_X = stof(strtoX);
                    //cout << "X= " << XX << endl;

                }
                if (type == "\r\nY:") {
                    //cout << "find Y data" << endl;
                    string strtoY;
                    strtoY = datastr.substr(4, datastr.find("|"));
                    rotate_Y = stof(strtoY);
                    //cout << "Y = " << YY << endl;

                }
                if (type == "\r\nZ:") {
                    //cout << "find Z data" << endl;
                    string strtoZ;
                    strtoZ = datastr.substr(4, datastr.find("|"));
                    rotate_Z = stof(strtoZ);
                    //cout << "Z = " << ZZ << endl;

                }
                break;
            }



        }

        //cout << adr << " " << rotate_X << " " << rotate_Y << " " << rotate_Z << endl;

    }

    return 0;
}