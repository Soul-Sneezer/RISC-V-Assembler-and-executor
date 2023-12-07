typedef struct
{
	char* value;
	Node* left, right;

	Node()
	{
		value = NULL;
		right = left = NULL;
	}

	Node(char* s)
	{
		value = (char*)malloc(sizeof(*s));
		value = s;
	}

} Node;

void newNode(char* value, Bit* pattern, int pattern_size)
{
	for(int i = 0; i < pattern
}
