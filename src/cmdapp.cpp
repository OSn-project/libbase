#include <stdio.h>
#include <base/string.h>
#include <base/cmdapp.h>

BCmdApp :: BCmdApp(BAppCommand *commands)
{
	this->commands = commands;

	this->prompt.set(" > ");
	this->quit_command = "quit";
	this->greeting = /*NULL*/"Welcome to the CHEESE FACTORY.\n";
}

BCmdApp :: ~BCmdApp()
{
}


void BCmdApp :: interactive(FILE *script)
{
	BString input;
	
	if (this->greeting)
		printf("%s", this->greeting);
	
	for (bool run = true; run; input.clear())
	{
		printf("%s", this->prompt.c_str());
		
		BString::read_line(stdin, &input);
		
		/* Check whether they want to quit */
		if (input.equals(this->quit_command))
		{
			run = false;
		}
		
		/* Check whether it's any of the given commands */
		for (BAppCommand *cmd = this->commands; cmd->function != NULL; cmd++)
		{
			if (input.equals(cmd->name))
			{
				if (cmd->function != NULL)
					cmd->function(NULL, NULL);
			}
		}
	}
}
