#ifndef LIBPLTAGGER_IO_WRITER_H
#define LIBPLTAGGER_IO_WRITER_H

#include <libpltagger/token.h>

#include <loki/Factory.h>
#include <loki/Singleton.h>

namespace PlTagger {

	/**
	 * Base class for writers of tagger tokens
	 */
	class TokenWriter
	{
	public:
		TokenWriter(std::ostream& os, const Tagset& tagset);

		virtual ~TokenWriter();

		virtual void write_token(const Token& t);

		void write_token_dispose(Token* t);

		virtual void write_sentence(const std::vector<Token*>& s);

		virtual void write_paragraph(const std::vector<std::vector<Token*> >& p);

		std::ostream& os() {
			return os_;
		}

		const Tagset& tagset() {
			return tagset_;
		}

		void finish();

		/**
		 * Factory interface for creating writers from string identifiers
		 *
		 * Mostly a convenience function to avoid having client code refer directly
		 * to the factory instance.
		 *
		 * @param class_id the unique class identifier
		 * @param os the output stream to pass to the writer's constructor
		 * @param tagset the tagset to pass to the writer's constructor
		 */
		static TokenWriter* create(const std::string class_id,
				std::ostream& os,
				const Tagset& tagset);

		/**
		 * Debug function to get a vector of available writer type strings.
		 */
		static std::vector<std::string> available_writer_types();

		/**
		 * Convenience template for registering TokenWriter derived classes.
		 */
		template <typename T>
		static bool register_writer(const std::string& class_id);


	protected:
		virtual void do_footer() {}

		virtual void do_token(const Token& t) = 0;

		virtual void do_sentence(const std::vector<Token*>& s) = 0;

		virtual void do_paragraph(const std::vector<std::vector<Token*> >& p) = 0;

		void indent_more(int n = 1);

		void indent_less(int n = 1);

		std::ostream& osi();

	private:
		std::ostream& os_;

		const Tagset& tagset_;

		bool needs_footer_;

		int indent_;
	};

	/**
	 * Declaration of the TokenWriter factory as a singleton Loki object factory.
	 * The factory instance can be accessed as TokenLayerFactory::Instance().
	 * It is assumed that all derived classes have the same constructor signature.
	 */
	typedef Loki::SingletonHolder<
		Loki::Factory<
			TokenWriter, // The base class for objects created in the factory
			std::string, // Identifier type
			Loki::TL::MakeTypelist< std::ostream&, const Tagset& >::Result
			// TokenLayer constructor arguments' types specification
		>,
		Loki::CreateUsingNew, // default_config, needed to change the item below
		Loki::LongevityLifetime::DieAsSmallObjectChild // Required per libloki docs
	>
	TokenWriterFactory;

	/**
	 * Convenience typedef for the exception type the factory throws
	 */
	typedef Loki::DefaultFactoryError<
		std::string, TokenWriter
	>::Exception
	TokenWriterFactoryException;

	/**
	 * Convenience template TokenWriter creation function
	 */
	template <typename T>
	inline
	T* writer_creator(std::ostream& os, const Tagset& tagset)
	{
		return new T(os, tagset);
	}

	template <typename T>
	bool TokenWriter::register_writer(const std::string& class_id)
	{
		bool ret = TokenWriterFactory::Instance().Register(class_id, writer_creator<T>);
		return ret;
	}

} /* end ns PlTagger */

#endif // LIBPLTAGGER_IO_WRITER_H