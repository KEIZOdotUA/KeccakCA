#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <ctime>
#include <iomanip>


using namespace std;


class RC
{
protected:
    bitset <64> RCArray [5][5];
    int RLength;

    RC(int hashLength)
    {
        switch (hashLength)
        {
            case 224:
            {
                RLength = 1152;
                break;
            }

            case 256:
            {
                RLength = 1088;
                break;
            }

            case 384:
            {
                RLength = 832;
                break;
            }

            default:
            {
                RLength = 576;
            }
        }

        int _count = 0;
        bitset <64> _temp[25] = {};

        for (int i = 0; i < RLength / 64; i++)
        {
            _temp[i].set();
            _temp[i].reset(i);
        }

        for (int row = 0; row < 5; row++)
        {
            for (int column = 0; column < 5; column++)
            {
                RCArray [row][column] = _temp[_count];
                _count++;
            }
        }

    }

    int GetPrevIndex(int currentIndex) {
        switch (currentIndex) {
            case 0: return 4;
            case 1: return 0;
            case 2: return 1;
            case 3: return 2;
            default: return 3;
        }
    }

    int GetNextIndex(int currentIndex) {
        switch (currentIndex) {
            case 0: return 1;
            case 1: return 2;
            case 2: return 3;
            case 3: return 4;
            default: return 0;
        }
    }

    void VectorCyclicShiftToLeft (bitset<64> &vectorToShift, int stepShift) {

        bitset <64> _tmp1;
        bitset <64> _tmp2;

        _tmp1 = vectorToShift << stepShift;
        _tmp2 = vectorToShift >> (64 - stepShift);

        vectorToShift = _tmp1 | _tmp2;

    }

    void VectorCyclicShiftToRight(bitset<64> &vectorToShift, int stepShift) {

        bitset <64> _tmp1;
        bitset <64> _tmp2;

        _tmp1 = vectorToShift >> stepShift;
        _tmp2 = vectorToShift << (64 - stepShift);

        vectorToShift = _tmp1 | _tmp2;

    }

    void ArrayShiftOZToLeft(bitset<64> (&arrayToShift)[5][5], int stepShift)
    {
        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                VectorCyclicShiftToLeft(arrayToShift[row][column], stepShift);
            }
        }
    }

    void ArrayShiftOZToRight(bitset<64> (&arrayToShift)[5][5], int stepShift)
    {
        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                VectorCyclicShiftToRight(arrayToShift[row][column], stepShift);
            }
        }
    }

    void ArrayShiftOXToLeft(bitset<64> (&arrayToShift)[5][5])
    {
        bitset <64> _temp [5][5];

        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                _temp[row][column] = arrayToShift [row][column];
            }
        }

        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                arrayToShift[row][column] = _temp [row][GetPrevIndex(column)];
            }
        }
    }

    void ArrayShiftOYToDown(bitset<64> (&arrayToShift)[5][5])
    {
        bitset <64> _temp [5][5];

        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                _temp[row][column] = arrayToShift [row][column];
            }
        }

        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                arrayToShift[row][column] = _temp [GetPrevIndex(row)][column];
            }
        }
    }

    void FirstColumnSourceToLastColumnTarget (bitset <64> (&sourceArray)[5][5], bitset <64> (&targetArray)[5][5])
    {
        for (int i = 0; i < 5; i++)
        {
            targetArray [i][4] = sourceArray [i][0];
        }
    }

    void Rule30()
    {
        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                bitset <64> _nord = RCArray[GetPrevIndex(row)][column];
                bitset <64> _south = RCArray[GetNextIndex(row)][column];
                bitset <64> _west = RCArray[row][GetPrevIndex(column)];
                bitset <64> _east = RCArray[row][GetNextIndex(column)];
                bitset <64> _face = RCArray[row][column];
                bitset <64> _back = RCArray[row][column];
                VectorCyclicShiftToRight(_face, 1);
                VectorCyclicShiftToLeft(_back, 1);

                RCArray[row][column] = (_nord xor _west xor _back) xor (RCArray[row][column] | (_south xor _east xor _face));
            }
        }
    }

    void Rule86()
    {
        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                bitset <64> _nord = RCArray[GetPrevIndex(row)][column];
                bitset <64> _south = RCArray[GetNextIndex(row)][column];
                bitset <64> _west = RCArray[row][GetPrevIndex(column)];
                bitset <64> _east = RCArray[row][GetNextIndex(column)];
                bitset <64> _face = RCArray[row][column];
                bitset <64> _back = RCArray[row][column];
                VectorCyclicShiftToRight(_face, 1);
                VectorCyclicShiftToLeft(_back, 1);

                RCArray[row][column] = (_nord xor _west xor _back) | RCArray[row][column] xor (_south xor _east xor _face);
            }
        }
    }

    void Rule150()
    {
        for (int row = 0; row < 5; row ++)
        {
            for (int column = 0; column < 5; column++)
            {
                bitset <64> _nord = RCArray[GetPrevIndex(row)][column];
                bitset <64> _south = RCArray[GetNextIndex(row)][column];
                bitset <64> _west = RCArray[row][GetPrevIndex(column)];
                bitset <64> _east = RCArray[row][GetNextIndex(column)];
                bitset <64> _face = RCArray[row][column];
                bitset <64> _back = RCArray[row][column];
                VectorCyclicShiftToRight(_face, 1);
                VectorCyclicShiftToLeft(_back, 1);

                RCArray[row][column] = (_nord xor _west xor _back) xor RCArray[row][column] xor (_south xor _east xor _face);
            }
        }
    }

    void FBlock()
    {
        for (int round = 0; round < 1; round++)		//Кількість раундів обробки
        {
            bitset <64> _newArrayRC [5][5];

            for (int i = 0; i < 5; i++)	//Заповнення копії масиву
            {
                Rule30();

                bitset <64> _shiftedCopyRC [5][5];

                for (int row = 0; row < 5; row++)
                {
                    for (int column = 0; column < 5; column++)
                    {
                        _shiftedCopyRC[row][column] = RCArray[row][column];
                    }
                }

                ArrayShiftOZToRight(_shiftedCopyRC, 23);

                for (int row = 0; row < 5; row++)
                {
                    for (int column = 0; column < 5; column++)
                    {
                        RCArray[row][column] = RCArray[row][column] xor _shiftedCopyRC[row][column];
                    }
                }

                Rule86();

                for (int row = 0; row < 5; row++)
                {
                    for (int column = 0; column < 5; column++)
                    {
                        _shiftedCopyRC[row][column] = RCArray[row][column];
                    }
                }

                ArrayShiftOZToLeft(_shiftedCopyRC, 3);

                for (int row = 0; row < 5; row++)
                {
                    for (int column = 0; column < 5; column++)
                    {
                        RCArray[row][column] = RCArray[row][column] xor _shiftedCopyRC[row][column];
                    }
                }

                Rule150();

                FirstColumnSourceToLastColumnTarget (RCArray, _newArrayRC);

                ArrayShiftOXToLeft(_newArrayRC);
                ArrayShiftOYToDown(_newArrayRC);

            }

            for (int row = 0; row < 5; row++)
            {
                for (int column = 0; column < 5; column++)
                {
                    RCArray[row][column] = _newArrayRC[row][column];
                }
            }

            Rule86();

            bitset <64> _shiftedCopyRC [5][5];

            for (int row = 0; row < 5; row++)
            {
                for (int column = 0; column < 5; column++)
                {
                    _shiftedCopyRC[row][column] = RCArray[row][column];
                }
            }

            ArrayShiftOZToLeft(_shiftedCopyRC, 3);

            for (int row = 0; row < 5; row++)
            {
                for (int column = 0; column < 5; column++)
                {
                    RCArray[row][column] = RCArray[row][column] xor _shiftedCopyRC[row][column];
                }
            }

            Rule150();

        }
    }
};



class Message : protected RC
{
protected:
    string TextMessage;
    string BinaryMessage;

    Message(int hashLen): RC(hashLen)
    {
        TextMessage = "";
        BinaryMessage = "";
    }

    void FileToStr (string pathToFile)
    {
        ifstream _file(pathToFile);
        if(_file.is_open())
        {
            TextMessage.assign((istreambuf_iterator<char>(_file.rdbuf())), istreambuf_iterator<char>());
            _file.close();
        }
        else {
            cout << "Unable to open file" << endl;
        }
    }

    void ASCIIToBits()
    {
        for (size_t i = 0; i < TextMessage.size(); ++i)
        {
            bitset<8> _symbol (unsigned(int(TextMessage.c_str()[i])));
            BinaryMessage += _symbol.to_string();
        }
    }

    void ContinuationOfMessage()
    {
        if (RLength - (BinaryMessage.length() % RLength) == 8)    //8 - довжина "|0х81|" в двійковій
        {
            BinaryMessage += "10000001";
        }
        else if (RLength - (BinaryMessage.length() % RLength) == 9)   //9 - довжина "|0x01||0x80|" в двійковій
        {
            BinaryMessage += "1";

            for (int i = 0; i < (2 * RLength - (BinaryMessage.length() % RLength)) - 8; i++)
            {
                BinaryMessage += "0";
            }
            BinaryMessage += "10000000";
        }
        else
        {
            BinaryMessage += "1";

            for (int i = 0; RLength - (BinaryMessage.length() % RLength) - 8; i++)
            {
                BinaryMessage += "0";
            }
            BinaryMessage += "10000000";
        }
    }
};

class Sponge : protected Message
{
public:

    int HashLenght;
    string Hash;

    Sponge(int hashLen): Message(hashLen)
    {
        HashLenght = hashLen;
        Hash = "";
    }

    string GetHexCharacter(string input) {

        stringstream _retValue;

        bitset<8> _bitset_input (input);

        _retValue << hex << uppercase << _bitset_input.to_ulong();

        return _retValue.str();

    }

    string BinToHex()
    {
        string _retValue;

        for (unsigned int i = 0; i < Hash.length(); i = i + 4) {
            _retValue += GetHexCharacter(Hash.substr(i, 4));
        }

        return _retValue;
    }

    void Init ()
    {

        FileToStr("E:\\File1.txt");
        int _noch = TextMessage.length();
        ASCIIToBits();
        ContinuationOfMessage();

        int _startTime =  clock();

        string _string_PartOfMessage;
        int _row = 0;
        int _column = 0;

        for (int i = 0; i < (BinaryMessage.length() / 64); i++)                                     //absorbing
        {
            _string_PartOfMessage = BinaryMessage.substr(unsigned(i * 64), 64);
            bitset <64> _bitset_PartOfMessage (_string_PartOfMessage);

            RCArray[_row][_column] = RCArray[_row][_column] xor _bitset_PartOfMessage;

            _column = GetNextIndex(_column);
            if (_column == 0)
            {
                _row = GetNextIndex(_row);
            }

            if ((i + 1) % (RLength / 64) == 0)
            {
                FBlock();
                _row = 0;
                _column = 0;
            }
        }

        int _endTime = clock();

        bitset <64> _bitset_PartOfHash;
        string _string_PartOfHash;

        for (int i = 0; i < HashLenght; i += 64)                                                           //squeezing
        {
            FBlock();

            _bitset_PartOfHash = RCArray[0][0];
            _string_PartOfHash = _bitset_PartOfHash.to_string<char, char_traits<char>, allocator<char> >();
            Hash += _string_PartOfHash;
        }

        Hash = Hash.substr(0, HashLenght);

        Hash = BinToHex();

        int _runTime = _endTime - _startTime;

        int _runTime_sec;

        if ((_runTime % 1000) > 495) {
            _runTime_sec = (_runTime / 1000) + 1;
        }
        else _runTime_sec = _runTime / 1000;

        int _runTime_min = _runTime_sec / 60;

        _runTime_sec = _runTime_sec - (_runTime_min * 60);

        float _float_noch = _noch;
        float _float_runTime = _runTime;
        float _chps;

        _chps = (_float_noch / _float_runTime) * 1000;

        cout << endl << "Hash: " << endl << Hash << endl << endl;
        cout << "Number of characters: " << _noch << endl;
        cout << "Runtime: " << _runTime << "ms " << "(" << _runTime_min <<"m " << _runTime_sec << "s)" << endl;
        cout << "Characters per second: " << fixed << setprecision(2) << _chps << endl << endl;

    }
};

int main() {

    int hashLen;

    cout << "Enter the length of hash: ";

    cin >> hashLen;

    if(hashLen > 127)
    {
        Sponge _sp(hashLen);
        _sp.Init();
    }
    else
        cout << endl << "Incorrect data" << endl << endl;

    system("pause");
    return 0;

}
