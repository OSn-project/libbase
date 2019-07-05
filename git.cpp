cmdspec = {
	.options = {
		{"-version", ARG_NONE},
		{NULL, NULL}
	};
	.subcmds = {
		{
			.name = "commit",
			.options = {
				{"amend", ARG_NONE},
				{"message", ARG_STRING}
				{NULL, NULL}
			}
			.callback = &Git::commit,
		}
	}
	.callback = &print_help();
	.bad_opt = NULL;
	.bad_cmd = NULL;
};

//.is_set()
//.get_string()

int main(int argc, char *argv)
{
	BCommand::execute(&cmdspec, argc, argv);
	// git -version
	// git commit amend
	return 0;
}

void Git :: commit()
{

}
