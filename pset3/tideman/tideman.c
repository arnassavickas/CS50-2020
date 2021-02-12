#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

//link checker
bool check_link(int win, int lose);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if ((strcmp(name, candidates[i])) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int nr = 0; //starts from pairs[0]
    for (int i = 1; i < candidate_count; i++) //starts from row 1
    {
        for (int j = 0; j < i; j++) //starts from column 0
        {
            if (preferences[i][j] > preferences [j][i])
            {
                pairs[nr].winner = i;
                pairs[nr].loser = j;
                nr++;                   //next check will be for next pairs[]
            }
            if (preferences[i][j] < preferences [j][i])
            {
                pairs[nr].winner = j;
                pairs[nr].loser = i;
                nr++;                    //next check will be for next pairs[]
            }
        }
    }
    pair_count = nr;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    bool check = false;
    do
    {
        for (int i = 1; i < pair_count; i++) //starts from sorted[1]
        {
            check = false;

            int win1 = pairs[i - 1].winner;
            int lose1 = pairs[i - 1].loser;
            int win2 = pairs[i].winner;
            int lose2 = pairs[i].loser;
            int tempwinner, temploser;

            if (preferences[win1][lose1] < preferences[win2][lose2])
            {
                tempwinner = pairs[i].winner;
                temploser = pairs[i].loser;

                pairs[i].winner = pairs[i - 1].winner;
                pairs[i].loser = pairs[i - 1].loser;

                pairs[i - 1].winner = tempwinner;
                pairs[i - 1].loser = temploser;
                check = true;
            }
        }
    }
    while (check == true);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (check_link(pairs[i].winner, pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

bool check_link(int win, int lose)
{
    if (locked[lose][win] == true)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][win] == true)
        {
            check_link(i, lose);
            if (check_link(i, lose) == true)
            {
                return true;
            }
        }
    }
    return false;
}


// Print the winner of the election
void print_winner(void)
{
    bool check;
    for (int j = 0; j < candidate_count; j++)
    {
        check = false;
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][j] == true)
            {
                check = true;
            }
        }
        if (check == false)
        {
            printf("%s\n", candidates[j]);
            return;
        }
    }
    return;
}

