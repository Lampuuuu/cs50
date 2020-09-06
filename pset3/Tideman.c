#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9


//represents the number of voters who prefer candidate i over candidate j
int preferences[MAX][MAX];

// locked[i][j] means candidate i is locked over candidate j
bool locked[MAX][MAX];

// Each pair has a winner, loser
// the integers are the candidate indexes
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count = 0;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(int a, int b);

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
    // all pairs for locked are now false
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

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


// returns bool
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        //checks validity of the vote and then ranks it in "ranks"
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;    //i being the index of the candidate 
            return true;
        }
    }
    return false;
}

// since "ranks" is changed for every voter, this records it in preferences
void record_preferences(int ranks[])
{
    // double loop: recording candidate i over candidate j
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}


// Record pairs of candidates where one is preferred over the other
// add it to the pairs count
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //if candidate i is preferred more over candidate j they will be recorded in the pairs array as winner and loser
            //the pair_count increases by 1
            if (preferences[i][j] - preferences[j][i] > 0)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        //swapping algorithm, swapping weaker pairs with stronger ones
        if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
        {
            int dummyw = pairs[i].winner;
            int dummyl = pairs[i].loser;
            pairs[i].winner = pairs[i + 1].winner;
            pairs[i].loser = pairs[i + 1].loser;
            pairs[i + 1].winner = dummyw;
            pairs[i + 1].loser = dummyl;
        }
    }
    return;
}



// initializing additional function, that will check if circles will be made by locking the pair
bool circlecheck(int win, int los);

void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        //if the pair passes the circlecheck it is locked in
        if (circlecheck(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true; 
        }
    }
    return;
}


bool circlecheck(int win, int los)
{
    //recursive function, that will keep the integer of the loser. The winner gets checked, if he lost to someone else and that winner gets checked again
    //if the original loser wins over any winner in that chain a cycle is created and the function will return false
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[los][win])
        {
            return false;
        }
        else if (locked[i][win])
        {
            circlecheck(i, los);
            if (!circlecheck(i, los))
            {
                return false;
            }
        }

    }
    return true;

}

//initializing another function and global variable needed to print the winner of the election
void winner(void);
int current_winner = 0;

void print_winner(void)
{
    //run winner function to get the current_winner integer to the correct spot and then printing the corresponding candidates string
    winner();
    printf("%s\n", candidates[current_winner]);
    return;
}


void winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // checks if anyone beats the current_winner, then increases their value by one and reruns
        // will rerun until no one beats the current_winner
        // there should be at least one winner, since cycles are avoided by the lock_pairs function
        if (locked[i][current_winner] == true)
        {
            current_winner += 1;
            winner();
        }
    }
}
