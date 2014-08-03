#include "lexertl/debug.hpp"
#include "lexertl/generator.hpp"
#include <iostream>
#include "lexertl/lookup.hpp"
#include <regexParse.h>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) struct dfa_graph_t *Regex_Parse(struct app_entry_t *apps)
{
	lexertl::rules rules;
    lexertl::state_machine sm;

	while(apps) {
		rules.push(apps->regex, apps->appId);
		apps = apps->next;
	}

	lexertl::generator::build(rules, sm);
	//sm.minimise();
	return lexertl::generator::reassemble(sm);
}

__declspec(dllexport) void Regex_Test(struct app_entry_t *apps, char *Text)
{
	lexertl::rules rules;
    lexertl::state_machine sm;

	while(apps) {
		rules.push(apps->regex, apps->appId);
		apps = apps->next;
	}

	lexertl::generator::build(rules, sm);
	sm.minimise();
	lexertl::debug::dump(sm, std::cout);

    std::string input(Text);
    lexertl::smatch results(input.begin(), input.end());

    // Read ahead
    lexertl::lookup(sm, results);

    while (results.id != 0)
    {
        std::cout << "Id: " << results.id << ", Token: '" <<
            results.str () << "'\n";
        lexertl::lookup(sm, results);
    }

	return;
}

#ifdef __cplusplus
}
#endif