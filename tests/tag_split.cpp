#include <boost/test/unit_test.hpp>
#include <set>
#include <libtoki/foreach.h>
#include "morph/sfstanalyser.h"

BOOST_AUTO_TEST_SUITE( tag_split )

struct F {
	F() {
		const char tagset_string[] = "[ATTR]\n"
			"A tag tog other a3 \n"
			"B data thing tag-thing thang\n"
			"C a b c \n"
			"[POS]\n some A B [C]\n";
		tagset.reset(new PlTagger::Tagset(tagset_string));
	}
	boost::shared_ptr<PlTagger::Tagset> tagset;

	void check_split(const char* orig, const std::set<std::string> expect)
	{
		std::set<std::string> actual;
		std::vector<PlTagger::Lexeme> lex;
		//try {
		std::string s = orig;
		s = "<" + s + ">";
		PlTagger::SfstAnalyser::split_analysis_into(s, lex, *tagset);
		//} catch (PlTagger::TagParseError& e) {
		//	BOOST_FAIL("Tag parse error! " << e.what());
		//}

		foreach (const PlTagger::Lexeme& l, lex) {
			BOOST_WARN(tagset->validate_tag(l.tag(), false));
			actual.insert(tagset->tag_to_string(l.tag()));
		}

		BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(), expect.begin(), expect.end());
	}
};


BOOST_FIXTURE_TEST_CASE( plain, F )
{
	const char tag[] = "some:tag:data";
	std::set<std::string> r;
	r.insert(tag);
	check_split(tag, r);
}

BOOST_FIXTURE_TEST_CASE( plus, F )
{
	const char tag[] = "some:tag:data+some:other:tag-thing";
	std::set<std::string> result;
	result.insert("some:tag:data");
	result.insert("some:other:tag-thing");
	check_split(tag, result);
}

BOOST_FIXTURE_TEST_CASE( dot, F )
{
	const char tag[] = "some:tag.tog:data";
	std::set<std::string> result;
	result.insert("some:tag:data");
	result.insert("some:tog:data");
	check_split(tag, result);
}

BOOST_FIXTURE_TEST_CASE( dots, F )
{
	const char tag[] = "some:tag.tog:data:a.b.c";
	std::set<std::string> result;
	result.insert("some:tag:data:a");
	result.insert("some:tog:data:a");
	result.insert("some:tag:data:b");
	result.insert("some:tog:data:b");
	result.insert("some:tag:data:c");
	result.insert("some:tog:data:c");
	check_split(tag, result);
}

BOOST_FIXTURE_TEST_CASE( dots_plus, F )
{
	const char tag[] = "some:tag.tog:data:a.b+some:other:thing.thang";
	std::set<std::string> result;
	result.insert("some:tag:data:a");
	result.insert("some:tog:data:a");
	result.insert("some:tag:data:b");
	result.insert("some:tog:data:b");
	result.insert("some:other:thing");
	result.insert("some:other:thang");
	check_split(tag, result);
}


BOOST_AUTO_TEST_SUITE_END()