/*
    Copyright (C) 2014 Radosław Warzocha, Adam Radziszewski
    Part of the libmaca project

    This program is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

    This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. 

    See the LICENSE.MACA, LICENSE.SFST, LICENSE.GUESSER, COPYING.LESSER and COPYING files for more details.
*/

#ifndef LIBMACA_MORFEUSZ2ANALYSER_H
#define LIBMACA_MORFEUSZ2ANALYSER_H

namespace Maca {

namespace details {
	struct MorfeuszEdge;
}

class MorfeuszAnalyser : public MorphAnalyser
{
public:
	/**
	 * Constructor for a Morfeusz analyser with a given tagset and converter.
	 * The tagset should be the output tagset of the converter.
	 */
	MorfeuszAnalyser(const Corpus2::Tagset* tagset,
			Conversion::TagsetConverter* conv);

	/**
	 * Config node constructor. Recognized keys are:
	 * - converter - the converter to load (from standard paths)
	 * - ign_tag - the tag to use when Morfeusz returns no analysis,
	 *             defaults to "ign"
	 * - warn_on_ign - warn when using the ign tag, false by default
	 * - warn_on_fold_failure - issue a warning when folding ambiguous paths
	 *                          is unsuccesful after conversion (off by def.)
	 */
	MorfeuszAnalyser(const Config::Node& cfg);

	/// Cloning
	MorfeuszAnalyser* clone() const;

	/// Destructor
	~MorfeuszAnalyser();

	/// MorphAnalyser override
	bool process_functional(const Toki::Token &t,
			boost::function<void(Corpus2::Token *)> sink);

	/// convert gathered tokens and pass them to the sink
	void flush_convert(std::vector<Corpus2::Token*>& vec,
			boost::function<void(Corpus2::Token *)> sink);

	/// convert gethered tokens (ambiguously segmented), try folding and
	/// pass the resulting tokens to the sink
	void flush_convert(std::vector< std::vector<Corpus2::Token*> >& vec,
			boost::function<void(Corpus2::Token *)> sink);

	/// Class identifier
	static const char* identifier;

	/// Registered flag
	static bool registered;

private:
	static const morfeusz::Charset charset;

	bool process_complex_analysis(const Toki::Token &t,
			std::vector<details::MorfeuszEdge>& pmorf,
			boost::function<void(Corpus2::Token *)>sink);

	/// helper to create a token from a Morfeusz interpretation struct
	Corpus2::Token* make_token(const Toki::Token& t,
			const details::MorfeuszEdge& m) const;

	/// helper to add lexemes from a Morfeusz interp struct into a token
	void morfeusz_into_token(Corpus2::Token* tt, const details::MorfeuszEdge& m) const;

	/// the tagset converter
	Conversion::TagsetConverter* conv_;

	Corpus2::Tag ign_tag_;

	bool warn_on_ign_;
	bool warn_on_fold_failure_;
};

namespace details {

/// Helper struct for holding preprocessed Morfeusz results
struct MorfeuszEdge
{
	explicit MorfeuszEdge(const morfeusz::MorphInterpretation& morf);

	int node_from, node_to;
	UnicodeString orth;
	UnicodeString lemma;
	std::string tag_string;
	Corpus2::Token* token;
};

} /* end ns details */

} /* end ns Maca */

#endif // LIBMACA_MORFEUSZ2ANALYSER_H
