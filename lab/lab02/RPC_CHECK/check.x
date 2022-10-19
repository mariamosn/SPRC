struct student {
	string name<>;
	string group<>;
};

program CHECKPROG {
	version CHECKVERS {
		string grade(struct student) = 1;
	} = 1;
} = 0x31234567;
