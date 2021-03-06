#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

unsigned int RC[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

unsigned int sbox[256] = {
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x1, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x4, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x5, 0x9a, 0x7, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x9, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x0, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x2, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0xc, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0xb, 0xdb, 0xe0, 0x32, 0x3a, 0xa, 0x49, 0x6, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x8, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x3, 0xf6, 0xe, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0xd, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0xf, 0xb0, 0x54, 0xbb, 0x16
};

unsigned int lookup_3[256] = {
  0x0 ,0x3 ,0x6 ,0x5 ,0xc ,0xf ,0xa ,0x9 ,0x18 ,0x1b ,0x1e ,0x1d ,0x14 ,0x17 ,0x12 ,0x11 ,0x30 ,0x33 ,0x36 ,0x35 ,0x3c ,0x3f ,0x3a ,0x39 ,0x28 ,0x2b ,0x2e ,0x2d ,0x24 ,0x27 ,0x22 ,0x21 ,0x60 ,0x63 ,0x66 ,0x65 ,0x6c ,0x6f ,0x6a ,0x69 ,0x78 ,0x7b ,0x7e ,0x7d ,0x74 ,0x77 ,0x72 ,0x71 ,0x50 ,0x53 ,0x56 ,0x55 ,0x5c ,0x5f ,0x5a ,0x59 ,0x48 ,0x4b ,0x4e ,0x4d ,0x44 ,0x47 ,0x42 ,0x41 ,0xc0 ,0xc3 ,0xc6 ,0xc5 ,0xcc ,0xcf ,0xca ,0xc9 ,0xd8 ,0xdb ,0xde ,0xdd ,0xd4 ,0xd7 ,0xd2 ,0xd1 ,0xf0 ,0xf3 ,0xf6 ,0xf5 ,0xfc ,0xff ,0xfa ,0xf9 ,0xe8 ,0xeb ,0xee ,0xed ,0xe4 ,0xe7 ,0xe2 ,0xe1 ,0xa0 ,0xa3 ,0xa6 ,0xa5 ,0xac ,0xaf ,0xaa ,0xa9 ,0xb8 ,0xbb ,0xbe ,0xbd ,0xb4 ,0xb7 ,0xb2 ,0xb1 ,0x90 ,0x93 ,0x96 ,0x95 ,0x9c ,0x9f ,0x9a ,0x99 ,0x88 ,0x8b ,0x8e ,0x8d ,0x84 ,0x87 ,0x82 ,0x81 ,0x9b ,0x98 ,0x9d ,0x9e ,0x97 ,0x94 ,0x91 ,0x92 ,0x83 ,0x80 ,0x85 ,0x86 ,0x8f ,0x8c ,0x89 ,0x8a ,0xab ,0xa8 ,0xad ,0xae ,0xa7 ,0xa4 ,0xa1 ,0xa2 ,0xb3 ,0xb0 ,0xb5 ,0xb6 ,0xbf ,0xbc ,0xb9 ,0xba ,0xfb ,0xf8 ,0xfd ,0xfe ,0xf7 ,0xf4 ,0xf1 ,0xf2 ,0xe3 ,0xe0 ,0xe5 ,0xe6 ,0xef ,0xec ,0xe9 ,0xea ,0xcb ,0xc8 ,0xcd ,0xce ,0xc7 ,0xc4 ,0xc1 ,0xc2 ,0xd3 ,0xd0 ,0xd5 ,0xd6 ,0xdf ,0xdc ,0xd9 ,0xda ,0x5b ,0x58 ,0x5d ,0x5e ,0x57 ,0x54 ,0x51 ,0x52 ,0x43 ,0x40 ,0x45 ,0x46 ,0x4f ,0x4c ,0x49 ,0x4a ,0x6b ,0x68 ,0x6d ,0x6e ,0x67 ,0x64 ,0x61 ,0x62 ,0x73 ,0x70 ,0x75 ,0x76 ,0x7f ,0x7c ,0x79 ,0x7a ,0x3b ,0x38 ,0x3d ,0x3e ,0x37 ,0x34 ,0x31 ,0x32 ,0x23 ,0x20 ,0x25 ,0x26 ,0x2f ,0x2c ,0x29 ,0x2a ,0xb ,0x8 ,0xd ,0xe ,0x7 ,0x4 ,0x1 ,0x2 ,0x13 ,0x10 ,0x15 ,0x16 ,0x1f ,0x1c ,0x19 ,0x1a
 };

 unsigned int lookup_2[256] = {
  0x0 ,0x2 ,0x4 ,0x6 ,0x8 ,0xa ,0xc ,0xe ,0x10 ,0x12 ,0x14 ,0x16 ,0x18 ,0x1a ,0x1c ,0x1e ,0x20 ,0x22 ,0x24 ,0x26 ,0x28 ,0x2a ,0x2c ,0x2e ,0x30 ,0x32 ,0x34 ,0x36 ,0x38 ,0x3a ,0x3c ,0x3e ,0x40 ,0x42 ,0x44 ,0x46 ,0x48 ,0x4a ,0x4c ,0x4e ,0x50 ,0x52 ,0x54 ,0x56 ,0x58 ,0x5a ,0x5c ,0x5e ,0x60 ,0x62 ,0x64 ,0x66 ,0x68 ,0x6a ,0x6c ,0x6e ,0x70 ,0x72 ,0x74 ,0x76 ,0x78 ,0x7a ,0x7c ,0x7e ,0x80 ,0x82 ,0x84 ,0x86 ,0x88 ,0x8a ,0x8c ,0x8e ,0x90 ,0x92 ,0x94 ,0x96 ,0x98 ,0x9a ,0x9c ,0x9e ,0xa0 ,0xa2 ,0xa4 ,0xa6 ,0xa8 ,0xaa ,0xac ,0xae ,0xb0 ,0xb2 ,0xb4 ,0xb6 ,0xb8 ,0xba ,0xbc ,0xbe ,0xc0 ,0xc2 ,0xc4 ,0xc6 ,0xc8 ,0xca ,0xcc ,0xce ,0xd0 ,0xd2 ,0xd4 ,0xd6 ,0xd8 ,0xda ,0xdc ,0xde ,0xe0 ,0xe2 ,0xe4 ,0xe6 ,0xe8 ,0xea ,0xec ,0xee ,0xf0 ,0xf2 ,0xf4 ,0xf6 ,0xf8 ,0xfa ,0xfc ,0xfe ,0x1b ,0x19 ,0x1f ,0x1d ,0x13 ,0x11 ,0x17 ,0x15 ,0xb ,0x9 ,0xf ,0xd ,0x3 ,0x1 ,0x7 ,0x5 ,0x3b ,0x39 ,0x3f ,0x3d ,0x33 ,0x31 ,0x37 ,0x35 ,0x2b ,0x29 ,0x2f ,0x2d ,0x23 ,0x21 ,0x27 ,0x25 ,0x5b ,0x59 ,0x5f ,0x5d ,0x53 ,0x51 ,0x57 ,0x55 ,0x4b ,0x49 ,0x4f ,0x4d ,0x43 ,0x41 ,0x47 ,0x45 ,0x7b ,0x79 ,0x7f ,0x7d ,0x73 ,0x71 ,0x77 ,0x75 ,0x6b ,0x69 ,0x6f ,0x6d ,0x63 ,0x61 ,0x67 ,0x65 ,0x9b ,0x99 ,0x9f ,0x9d ,0x93 ,0x91 ,0x97 ,0x95 ,0x8b ,0x89 ,0x8f ,0x8d ,0x83 ,0x81 ,0x87 ,0x85 ,0xbb ,0xb9 ,0xbf ,0xbd ,0xb3 ,0xb1 ,0xb7 ,0xb5 ,0xab ,0xa9 ,0xaf ,0xad ,0xa3 ,0xa1 ,0xa7 ,0xa5 ,0xdb ,0xd9 ,0xdf ,0xdd ,0xd3 ,0xd1 ,0xd7 ,0xd5 ,0xcb ,0xc9 ,0xcf ,0xcd ,0xc3 ,0xc1 ,0xc7 ,0xc5 ,0xfb ,0xf9 ,0xff ,0xfd ,0xf3 ,0xf1 ,0xf7 ,0xf5 ,0xeb ,0xe9 ,0xef ,0xed ,0xe3 ,0xe1 ,0xe7 ,0xe5
 };

unsigned int fixed_matrix[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};


unsigned int char_to_hex(char x)
{
    if(x >='0' && x <= '9') return (x - '0');
    else if(x >= 'a' && x <= 'f') return (10 + (x - 'a'));
    else if(x >= 'A' && x <= 'F') return (10 + (x- 'A'));
}

int main()
{
    // The user will enter those values via stdin
    string plaintext_input;
    string key_input;
    int inputs;
    int encryp_reps;

    // Procedure will work to transform the user inputs into the following arrays
    // Those arrays will be the unit which we will manipulate through the code
    unsigned int ciphertext[16];
    unsigned int state_array[4][4];
    unsigned int mixed_state_array[4][4];
    unsigned int expanded_key[11][4][4];


    cin >> inputs;


    for(int i = 0; i < inputs; i++) {
    cin >> plaintext_input;
    cin >> key_input;
    cin >> encryp_reps;

    //Preparation of the zero round key (initial key) which is the zeroth round key in
    //the expanded key
    int k = 0;
    unsigned int cell_left;
    unsigned int cell_right;
    for(int j = 0; j < 4; j++)
    {
        for(int i = 0; i < 4; i++)
        {
            cell_left = char_to_hex(key_input[k]);
            cell_right = char_to_hex(key_input[k+1]);
            expanded_key[0][i][j] = (cell_left << 4)^(cell_right);
            k = k + 2;
        }
    }

    //Input preparation: the following code will place the user input plaintext
    //into the state_array but first converting each two characters from the input stream
    // to two hexa characters. the two hexa characters will be put in once cell of
    // the state_array
    k = 0;
    for(int j = 0; j < 4; j++)
    {
        for(int i = 0; i < 4; i++)
        {
          cell_left = char_to_hex(plaintext_input[k]);
          cell_right = char_to_hex(plaintext_input[k+1]);
          state_array[i][j] = (cell_left << 4)^(cell_right);
          k = k + 2;
        }
    }


    //Key expansion: here the 4x4 key array will be expanded to be
    //a 4x44 round byte key array
    //each 4 columns of the expanded key array will make one round key
    //the next round will take the next 4 columns, etc.
    unsigned int temp[4];
    for(int kk = 1; kk < 11; kk++)
    {
        //rotating the column
        temp[0] = sbox[expanded_key[kk-1][1][3]]^RC[kk-1];
        temp[1] = sbox[expanded_key[kk-1][2][3]];
        temp[2] = sbox[expanded_key[kk-1][3][3]];
        temp[3] = sbox[expanded_key[kk-1][0][3]];

        expanded_key[kk][0][0] = temp[0]^expanded_key[kk-1][0][0];
        expanded_key[kk][1][0] = temp[1]^expanded_key[kk-1][1][0];
        expanded_key[kk][2][0] = temp[2]^expanded_key[kk-1][2][0];
        expanded_key[kk][3][0] = temp[3]^expanded_key[kk-1][3][0];

        for(int i = 1; i < 4; i++)
        {
            expanded_key[kk][0][i] = expanded_key[kk][0][i-1]^expanded_key[kk-1][0][i];
            expanded_key[kk][1][i] = expanded_key[kk][1][i-1]^expanded_key[kk-1][1][i];
            expanded_key[kk][2][i] = expanded_key[kk][2][i-1]^expanded_key[kk-1][2][i];
            expanded_key[kk][3][i] = expanded_key[kk][3][i-1]^expanded_key[kk-1][3][i];
        }

    }

    for (int i = 0; i < encryp_reps; i++){
    //The rounds will start and the state array will be mutated in each round
    //after the while loops finishes, the state array will be the ciphertext we want
    //so after the while loop we have two options, either to print the cipher text
    //or encrypt this cipher text again with the same expanded key
    //for the second option the

    //Adding round key 0 to the initial state array
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            state_array[i][j] = state_array[i][j]^expanded_key[0][i][j];
            //cout << hex << state_array[i][j] << " ";
        }
            //cout << "\n";
    }
    int round = 1;
    while(round < 11)
    {
        //All the shit will be done here

        //First the substitute bytes stage. This will mutate the state_array
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                state_array[i][j] = sbox[state_array[i][j]];
            }
        }

        //Second, the shift rows stage
        for(int i = 1; i < 4; i++)
        {
            unsigned int temp1, temp2;
            if(i == 1)
            {
                temp1 = state_array[i][0];
                state_array[i][0] = state_array[i][1];
                state_array[i][1] = state_array[i][2];
                state_array[i][2] = state_array[i][3];
                state_array[i][3] = temp1;
            }
            else if(i == 2)
            {
                temp1 = state_array[i][0];
                state_array[i][0] = state_array[i][2];
                temp2 = state_array[i][1];
                state_array[i][1] = state_array[i][3];
                state_array[i][2] = temp1;
                state_array[i][3] = temp2;
            }
            else if(i == 3)
            {
                temp1 = state_array[i][3];
                state_array[i][3] = state_array[i][2];
                state_array[i][2] = state_array[i][1];
                state_array[i][1] = state_array[i][0];
                state_array[i][0] = temp1;
            }
        }

        //Third: Mix columns stage
        if(round != 10)
        {
            for (int m = 0; m < 4; m++)
            {
                for(int n = 0; n < 4; n++)
                {
                    if (m == 0) {
                        mixed_state_array[m][n] = lookup_2[state_array[0][n]]
                        ^lookup_3[state_array[1][n]]
                        ^state_array[2][n]
                        ^state_array[3][n];
                    }

                    else if (m == 1) {
                        mixed_state_array[m][n] = state_array[0][n]
                        ^lookup_2[state_array[1][n]]
                        ^lookup_3[state_array[2][n]]
                        ^state_array[3][n];
                    }
                    else if (m == 2) {
                        mixed_state_array[m][n] = state_array[0][n]
                        ^state_array[1][n]
                        ^lookup_2[state_array[2][n]]
                        ^lookup_3[state_array[3][n]];
                    }
                    else if (m == 3) {
                        mixed_state_array[m][n] = lookup_3[state_array[0][n]]
                        ^state_array[1][n]
                        ^state_array[2][n]
                        ^lookup_2[state_array[3][n]];
                    }

                }
            }


            //assigning the state array the value of the mixed state array
            //so we can process it further in the program
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    state_array[i][j] = mixed_state_array[i][j];
                }
            }
        }

        //Adding round key stage
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                    state_array[i][j] = state_array[i][j]^expanded_key[round][i][j];
            }
        }
        round++;
    }


    }


    for(int j = 0; j < 4; j++)
    {
        for(int i = 0; i < 4; i++)
        {
                cout << setfill('0') << setw(2) << hex << state_array[i][j];
        }
    }
    cout << "\r\n";
    }

    return 0;
}
