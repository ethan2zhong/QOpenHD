// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CSV_CSV_OPTIONS_HPP
#define JSONCONS_CSV_CSV_OPTIONS_HPP

#include <string>
#include <vector>
#include <utility> // std::pair
#include <unordered_map> // std::unordered_map
#include <limits> // std::numeric_limits
#include <cwchar>
#include <jsoncons/json_options.hpp>

namespace jsoncons { namespace csv {

namespace detail {
    JSONCONS_STRING_LITERAL(string,'s','t','r','i','n','g')
    JSONCONS_STRING_LITERAL(integer,'i','n','t','e','g','e','r')
    JSONCONS_STRING_LITERAL(float,'f','l','o','a','t')
    JSONCONS_STRING_LITERAL(boolean,'b','o','o','l','e','a','n')
}

enum class csv_column_type
{
    string_t,integer_t,float_t,boolean_t,repeat_t
};

enum class quote_style_kind
{
    minimal,all,nonnumeric,none
};

enum class mapping_strategy
{
    n_rows, 
    n_objects, 
    m_columns
};

#if !defined(JSONCONS_NO_DEPRECATED)
JSONCONS_DEPRECATED_MSG("Instead, use quote_style_kind") typedef quote_style_kind quote_styles;
JSONCONS_DEPRECATED_MSG("Instead, use quote_style_kind") typedef quote_style_kind quote_style_type;
JSONCONS_DEPRECATED_MSG("Instead, use mapping_strategy") typedef mapping_strategy mapping_type;
#endif

enum class column_state {sequence,label};

struct csv_type_info
{
    csv_type_info() = default;
    csv_type_info(const csv_type_info&) = default;
    csv_type_info(csv_type_info&&) = default;

    csv_type_info(csv_column_type ctype, size_t lev, size_t repcount = 0)
    {
        col_type = ctype;
        level = lev;
        rep_count = repcount;
    }

    csv_column_type col_type;
    size_t level;
    size_t rep_count;
};

template <class CharT>
class basic_csv_options_common 
{
public:
    typedef CharT char_type;
    typedef std::basic_string<CharT> string_type;
protected:
    char_type field_delimiter_;
    std::pair<char_type,bool> subfield_delimiter_;
    char_type quote_char_;
    char_type quote_escape_char_;
    std::vector<string_type> column_names_;

    basic_csv_options_common()
        : field_delimiter_(','),
          subfield_delimiter_(std::make_pair(',',false)),
          quote_char_('\"'),
          quote_escape_char_('\"')
    {
    }

    basic_csv_options_common(const basic_csv_options_common&) = default;
    basic_csv_options_common& operator=(const basic_csv_options_common&) = default;

    virtual ~basic_csv_options_common() = default;
public:

    char_type field_delimiter() const 
    {
        return field_delimiter_;
    }

    const std::pair<char_type,bool>& subfield_delimiter() const 
    {
        return subfield_delimiter_;
    }

    char_type quote_char() const 
    {
        return quote_char_;
    }

    char_type quote_escape_char() const 
    {
        return quote_escape_char_;
    }

    std::vector<string_type> column_names() const 
    {
        return column_names_;
    }
};

template <class CharT>
class basic_csv_decode_options : public virtual basic_csv_options_common<CharT>
{
    using super_type = basic_csv_options_common<CharT>;
public:
    using typename super_type::char_type;
    using typename super_type::string_type;
protected:
    size_t header_lines_;
    bool assume_header_;
    bool ignore_empty_values_;
    bool ignore_empty_lines_;
    bool trim_leading_;
    bool trim_trailing_;
    bool trim_leading_inside_quotes_;
    bool trim_trailing_inside_quotes_;
    bool unquoted_empty_value_is_null_;
    bool infer_types_;
    bool lossless_number_;
    char_type comment_starter_;
    std::pair<mapping_strategy,bool> mapping_strategy_;
    unsigned long max_lines_;
    std::vector<csv_type_info> column_types_;
    std::vector<string_type> column_defaults_;
public:
    basic_csv_decode_options()
        : header_lines_(0),
          assume_header_(false),
          ignore_empty_values_(false),
          ignore_empty_lines_(true),
          trim_leading_(false),
          trim_trailing_(false),
          trim_leading_inside_quotes_(false),
          trim_trailing_inside_quotes_(false),
          unquoted_empty_value_is_null_(false),
          infer_types_(true),
          lossless_number_(false),
          comment_starter_('\0'),
          mapping_strategy_({mapping_strategy::n_rows,false}),
          max_lines_((std::numeric_limits<unsigned long>::max)())
    {}

    size_t header_lines() const 
    {
        return (assume_header_ && header_lines_ <= 1) ? 1 : header_lines_;
    }

    bool assume_header() const 
    {
        return assume_header_;
    }

    bool ignore_empty_values() const 
    {
        return ignore_empty_values_;
    }

    bool ignore_empty_lines() const 
    {
        return ignore_empty_lines_;
    }

    bool trim_leading() const 
    {
        return trim_leading_;
    }

    bool trim_trailing() const 
    {
        return trim_trailing_;
    }

    bool trim_leading_inside_quotes() const 
    {
        return trim_leading_inside_quotes_;
    }

    bool trim_trailing_inside_quotes() const 
    {
        return trim_trailing_inside_quotes_;
    }

    bool trim() const 
    {
        return trim_leading_ && trim_trailing_;
    }

    bool trim_inside_quotes() const 
    {
        return trim_leading_inside_quotes_ && trim_trailing_inside_quotes_;
    }

    bool unquoted_empty_value_is_null() const 
    {
        return unquoted_empty_value_is_null_;
    }

    bool infer_types() const 
    {
        return infer_types_;
    }

    bool lossless_number() const 
    {
        return lossless_number_;
    }

    char_type comment_starter() const 
    {
        return comment_starter_;
    }

    mapping_strategy mapping() const 
    {
        return mapping_strategy_.second ? (mapping_strategy_.first) : (assume_header() || this->column_names_.size() > 0 ? mapping_strategy::n_objects : mapping_strategy::n_rows);
    }

    unsigned long max_lines() const 
    {
        return max_lines_;
    }

    std::vector<csv_type_info> column_types() const 
    {
        return column_types_;
    }

    std::vector<string_type> column_defaults() const 
    {
        return column_defaults_;
    }
};

template <class CharT>
class basic_csv_encode_options : public virtual basic_csv_options_common<CharT>
{
    using super_type = basic_csv_options_common<CharT>;
public:
    using typename super_type::char_type;
    using typename super_type::string_type;
protected:
    quote_style_kind quote_style_;
    float_chars_format float_format_;
    int precision_;
    string_type line_delimiter_;
public:
    basic_csv_encode_options()
      : quote_style_(quote_style_kind::minimal),
        float_format_(float_chars_format::general),
        precision_(0)
    {
        line_delimiter_.push_back('\n');
    }

    quote_style_kind quote_style() const 
    {
        return quote_style_;
    }

    float_chars_format float_format() const 
    {
        return float_format_;
    }

    int precision() const 
    {
        return precision_;
    }

    string_type line_delimiter() const
    {
        return line_delimiter_;
    }
};

template <class CharT>
class basic_csv_options final : public basic_csv_decode_options<CharT>, public basic_csv_encode_options<CharT>  
{
    typedef CharT char_type;
    typedef std::basic_string<CharT> string_type;

public:
    using basic_csv_decode_options<CharT>::field_delimiter;
    using basic_csv_decode_options<CharT>::subfield_delimiter;
    using basic_csv_decode_options<CharT>::quote_char;
    using basic_csv_decode_options<CharT>::quote_escape_char;
    using basic_csv_decode_options<CharT>::column_names;
    using basic_csv_decode_options<CharT>::header_lines; 
    using basic_csv_decode_options<CharT>::assume_header; 
    using basic_csv_decode_options<CharT>::ignore_empty_values; 
    using basic_csv_decode_options<CharT>::ignore_empty_lines; 
    using basic_csv_decode_options<CharT>::trim_leading; 
    using basic_csv_decode_options<CharT>::trim_trailing; 
    using basic_csv_decode_options<CharT>::trim_leading_inside_quotes; 
    using basic_csv_decode_options<CharT>::trim_trailing_inside_quotes; 
    using basic_csv_decode_options<CharT>::trim; 
    using basic_csv_decode_options<CharT>::trim_inside_quotes; 
    using basic_csv_decode_options<CharT>::unquoted_empty_value_is_null; 
    using basic_csv_decode_options<CharT>::infer_types; 
    using basic_csv_decode_options<CharT>::lossless_number; 
    using basic_csv_decode_options<CharT>::comment_starter; 
    using basic_csv_decode_options<CharT>::mapping; 
    using basic_csv_decode_options<CharT>::max_lines; 
    using basic_csv_decode_options<CharT>::column_types; 
    using basic_csv_decode_options<CharT>::column_defaults; 
    using basic_csv_encode_options<CharT>::float_format;
    using basic_csv_encode_options<CharT>::precision;
    using basic_csv_encode_options<CharT>::line_delimiter;
    using basic_csv_encode_options<CharT>::quote_style;

    static const size_t default_indent = 4;

//  Constructors

    basic_csv_options()
    {
    }

    basic_csv_options& float_format(float_chars_format value)
    {
        this->float_format_ = value;
        return *this;
    }

    basic_csv_options& precision(int value)
    {
        this->precision_ = value;
        return *this;
    }

    basic_csv_options& header_lines(size_t value)
    {
        this->header_lines_ = value;
        return *this;
    }

    basic_csv_options& assume_header(bool value)
    {
        this->assume_header_ = value;
        return *this;
    }

    basic_csv_options& ignore_empty_values(bool value)
    {
        this->ignore_empty_values_ = value;
        return *this;
    }

    basic_csv_options& ignore_empty_lines(bool value)
    {
        this->ignore_empty_lines_ = value;
        return *this;
    }

    basic_csv_options& trim_leading(bool value)
    {
        this->trim_leading_ = value;
        return *this;
    }

    basic_csv_options& trim_trailing(bool value)
    {
        this->trim_trailing_ = value;
        return *this;
    }

    basic_csv_options& trim_leading_inside_quotes(bool value)
    {
        this->trim_leading_inside_quotes_ = value;
        return *this;
    }

    basic_csv_options& trim_trailing_inside_quotes(bool value)
    {
        this->trim_trailing_inside_quotes_ = value;
        return *this;
    }

    basic_csv_options& trim(bool value)
    {
        this->trim_leading_ = value;
        this->trim_trailing_ = value;
        return *this;
    }

    basic_csv_options& trim_inside_quotes(bool value)
    {
        this->trim_leading_inside_quotes_ = value;
        this->trim_trailing_inside_quotes_ = value;
        return *this;
    }

    basic_csv_options& unquoted_empty_value_is_null(bool value)
    {
        this->unquoted_empty_value_is_null_ = value;
        return *this;
    }

    basic_csv_options& column_names(const string_type& names)
    {
        this->column_names_ = parse_column_names(names);
        return *this;
    }

    basic_csv_options& column_types(const string_type& types)
    {
        this->column_types_ = parse_column_types(types);
        return *this;
    }

    basic_csv_options& column_defaults(const string_type& defaults)
    {
        this->column_defaults_ = parse_column_names(defaults);
        return *this;
    }

    basic_csv_options& field_delimiter(char_type value)
    {
        this->field_delimiter_ = value;
        return *this;
    }

    basic_csv_options& subfield_delimiter(char_type value)
    {
        this->subfield_delimiter_ = std::make_pair(value,true);
        return *this;
    }

    basic_csv_options& line_delimiter(string_type value)
    {
        this->line_delimiter_ = value;
        return *this;
    }

    basic_csv_options& quote_char(char_type value)
    {
        this->quote_char_ = value;
        return *this;
    }

    basic_csv_options& infer_types(bool value)
    {
        this->infer_types_ = value;
        return *this;
    }

    basic_csv_options& lossless_number(bool value) 
    {
        this->lossless_number_ = value;
        return *this;
    }

    basic_csv_options& quote_escape_char(char_type value)
    {
        this->quote_escape_char_ = value;
        return *this;
    }

    basic_csv_options& comment_starter(char_type value)
    {
        this->comment_starter_ = value;
        return *this;
    }

    basic_csv_options& quote_style(quote_style_kind value)
    {
        this->quote_style_ = value;
        return *this;
    }

    basic_csv_options& mapping(mapping_strategy value)
    {
        this->mapping_strategy_ = {value,true};
        return *this;
    }

    basic_csv_options& max_lines(unsigned long value)
    {
        this->max_lines_ = value;
        return *this;
    }

#if !defined(JSONCONS_NO_DEPRECATED)

    JSONCONS_DEPRECATED_MSG("Instead, use float_format(float_chars_format)")
    basic_csv_options& floating_point_format(float_chars_format value)
    {
        this->float_format_ = value;
        return *this;
    }

    JSONCONS_DEPRECATED_MSG("Instead, use column_names(const string_type&)")
    basic_csv_options& column_names(const std::vector<string_type>& value)
    {
        this->column_names_ = value;
        return *this;
    }

    JSONCONS_DEPRECATED_MSG("Instead, use column_defaults(const string_type&)")
    basic_csv_options& column_defaults(const std::vector<string_type>& value)
    {
        this->column_defaults_ = value;
        return *this;
    }

    JSONCONS_DEPRECATED_MSG("Instead, use column_types(const string_type&)")
    basic_csv_options& column_types(const std::vector<string_type>& value)
    {
        if (value.size() > 0)
        {
            this->column_types_.reserve(value.size());
            for (size_t i = 0; i < value.size(); ++i)
            {
                if (value[i] == jsoncons::csv::detail::string_literal<char_type>()())
                {
                    this->column_types_.emplace_back(csv_column_type::string_t,0);
                }
                else if (value[i] == jsoncons::csv::detail::integer_literal<char_type>()())
                {
                    this->column_types_.emplace_back(csv_column_type::integer_t,0);
                }
                else if (value[i] == jsoncons::csv::detail::float_literal<char_type>()())
                {
                    this->column_types_.emplace_back(csv_column_type::float_t,0);
                }
                else if (value[i] == jsoncons::csv::detail::boolean_literal<char_type>()())
                {
                    this->column_types_.emplace_back(csv_column_type::boolean_t,0);
                }
            }
        }
        return *this;
    }
#endif

    static std::vector<string_type> parse_column_names(const string_type& names)
    {
        std::vector<string_type> column_names;

        column_state state = column_state::sequence;
        string_type buffer;

        auto p = names.begin();
        while (p != names.end())
        {
            switch (state)
            {
                case column_state::sequence:
                {
                    switch (*p)
                    {
                        case ' ': case '\t':case '\r': case '\n':
                            ++p;
                            break;
                        default:
                            buffer.clear();
                            state = column_state::label;
                            break;
                    }
                    break;
                }
                case column_state::label:
                {
                    switch (*p)
                    {
                    case ',':
                        column_names.push_back(buffer);
                        buffer.clear();
                        ++p;
                        state = column_state::sequence;
                        break;
                    default:
                        buffer.push_back(*p);
                        ++p;
                        break;
                    }
                    break;
                }
            }
        }
        if (state == column_state::label)
        {
            column_names.push_back(buffer);
            buffer.clear();
        }
        return column_names;
    }

    static std::vector<csv_type_info> parse_column_types(const string_type& types)
    {
        const std::unordered_map<string_type,csv_column_type, std::hash<string_type>,std::equal_to<string_type>> type_dictionary =
        {

            {detail::string_literal<char_type>(),csv_column_type::string_t},
            {detail::integer_literal<char_type>(),csv_column_type::integer_t},
            {detail::float_literal<char_type>(),csv_column_type::float_t},
            {detail::boolean_literal<char_type>(),csv_column_type::boolean_t}
        };

        std::vector<csv_type_info> column_types;

        column_state state = column_state::sequence;
        int depth = 0;
        string_type buffer;

        auto p = types.begin();
        while (p != types.end())
        {
            switch (state)
            {
                case column_state::sequence:
                {
                    switch (*p)
                    {
                    case ' ': case '\t':case '\r': case '\n':
                        ++p;
                        break;
                    case '[':
                        ++depth;
                        ++p;
                        break;
                    case ']':
                        JSONCONS_ASSERT(depth > 0);
                        --depth;
                        ++p;
                        break;
                    case '*':
                        {
                            JSONCONS_ASSERT(column_types.size() != 0);
                            size_t offset = 0;
                            size_t level = column_types.size() > 0 ? column_types.back().level: 0;
                            if (level > 0)
                            {
                                for (auto it = column_types.rbegin();
                                     it != column_types.rend() && level == it->level;
                                     ++it)
                                {
                                    ++offset;
                                }
                            }
                            else
                            {
                                offset = 1;
                            }
                            column_types.emplace_back(csv_column_type::repeat_t,depth,offset);
                            ++p;
                            break;
                        }
                    default:
                        buffer.clear();
                        state = column_state::label;
                        break;
                    }
                    break;
                }
                case column_state::label:
                {
                    switch (*p)
                    {
                        case '*':
                        {
                            auto it = type_dictionary.find(buffer);
                            if (it != type_dictionary.end())
                            {
                                column_types.emplace_back(it->second,depth);
                                buffer.clear();
                            }
                            else
                            {
                                JSONCONS_ASSERT(false);
                            }
                            state = column_state::sequence;
                            break;
                        }
                        case ',':
                        {
                            auto it = type_dictionary.find(buffer);
                            if (it != type_dictionary.end())
                            {
                                column_types.emplace_back(it->second,depth);
                                buffer.clear();
                            }
                            else
                            {
                                JSONCONS_ASSERT(false);
                            }
                            ++p;
                            state = column_state::sequence;
                            break;
                        }
                        case ']':
                        {
                            JSONCONS_ASSERT(depth > 0);
                            auto it = type_dictionary.find(buffer);
                            if (it != type_dictionary.end())
                            {
                                column_types.emplace_back(it->second,depth);
                                buffer.clear();
                            }
                            else
                            {
                                JSONCONS_ASSERT(false);
                            }
                            --depth;
                            ++p;
                            state = column_state::sequence;
                            break;
                        }
                        default:
                        {
                            buffer.push_back(*p);
                            ++p;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        if (state == column_state::label)
        {
            auto it = type_dictionary.find(buffer);
            if (it != type_dictionary.end())
            {
                column_types.emplace_back(it->second,depth);
                buffer.clear();
            }
            else
            {
                JSONCONS_ASSERT(false);
            }
        }
        return column_types;
    }

};

typedef basic_csv_options<char> csv_options;
typedef basic_csv_options<wchar_t> wcsv_options;

#if !defined(JSONCONS_NO_DEPRECATED)
JSONCONS_DEPRECATED_MSG("Instead, use csv_options") typedef csv_options csv_parameters;
JSONCONS_DEPRECATED_MSG("Instead, use wcsv_options") typedef wcsv_options wcsv_parameters;
JSONCONS_DEPRECATED_MSG("Instead, use csv_options") typedef csv_options csv_serializing_options;
JSONCONS_DEPRECATED_MSG("Instead, use wcsv_options") typedef wcsv_options wcsv_serializing_options;
#endif


}}
#endif
