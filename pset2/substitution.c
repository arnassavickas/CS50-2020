#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

//get key
//validate key
//  check key length (26)
//  check non-alphabetic
//  check repeated characters
//get plaintext
//encipher
//  preserve case
//  leave non-alphabetic not ciphered
//print ciphertext
bool validate(string key);
string encipher(string key, string plain);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        bool validation = validate(argv[1]);
        if (validation == true)
        {
            string pt = get_string("plaintext: ");
            string ct = encipher(argv[1], pt);
            printf("ciphertext: %s\n", ct);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

//    printf("ciphertext: %s", ct)
}

bool validate(string key)
{
    bool test = true;
    if (key[26] != '\0')
    {
        printf("Key must contain 26 characters.\n");
        test = false;
    }
    else
    {
        bool check[127] = {false};
        for (int i = 0; key[i] != '\0'; i++)
        {
            if (key[i] < 65 || (key[i] > 90 && key[i] < 97) || key[i] > 122)
            {
                printf("Key must only contain alphabetic characters.\n");
                test = false;
                key[i + 1] = '\0';
            }
            int a = key[i];
            if (check[a] == true)
            {
                printf("Key must not contain repeating characters.\n");
                test = false;
                key[i + 1] = '\0';
            }
            check[a] = true;
        }
    }
    return test;
}

string encipher(string key, string plain)
{
    int keylength = strlen(key);
    int upperkey[keylength], lowerkey[keylength];
    for (int i = 0; key[i] != '\0'; i++)
    {
        upperkey[i] = toupper(key[i]);
        lowerkey[i] = tolower(key[i]);
    }
    
    for (int i = 0; plain[i] != '\0'; i++)    //goes through plaintext letters i from 0 to the end
    {
//for i letter goes through alphabeth and checks if it's 'a' (then b, c, d...) and if yes, encrypts it to the k key letter
        for (int up = 65, low = 97, k = 0; up < 91 && low < 123; up++, low++, k++)
        {
            if (plain[i] == up)
            {
                plain[i] = upperkey[k];
                up = 91;
            }
            else if (plain[i] == low)
            {
                plain[i] = lowerkey[k];
                low = 123;
            }
        }
    }
    return plain;
}