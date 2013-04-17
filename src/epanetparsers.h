#ifndef WaterObjects_epanetparsers_h
#define WaterObjects_epanetparsers_h

#define BOOST_SPIRIT_DEBUG

#define WORDLEX_STR "._a-zA-Z0-9"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_parse_attr.hpp>
#include <boost/fusion/adapted.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using std::string;
using boost::spirit::lexeme;
using qi::double_;
using qi::char_;
using ascii::space;
using ascii::space_type;
using ascii::alnum;
using qi::rule;
using qi::lit;
using qi::eol;
using qi::grammar;

#define BOOST_SPIRIT_DEBUG

#pragma mark Parsed data structures
namespace WaterObjects {
  
  struct parsedJunction {
    string id;
    double elevation;
    double demand;
    string pattern;
    string description;
  };
  
  struct parsedReservoir {
    string id;
    double head;
    string pattern;
    string description;
  };
  
  struct parsedTank {
    string id;
    double elevation;
    double initLevel;
    double minLevel;
    double maxLevel;
    double diameter;
    double minVol;
    string volumeCurve;
    string description;
  };
  
  struct parsedPipe {
    string id;
    string node1;
    string node2;
    double length;
    double diameter;
    double roughness;
    double minorLoss;
    string status;
    string description;
  };
  
  struct parsedPump {
    string id;
    string node1;
    string node2;
    string parameters;
    string description;
  };
  
  struct parsedValve {
    string id;
    string node1;
    string node2;
    double diameter;
    string type;
    string info; // pressure setting, flow setting, headloss coef, or headloss curve ID
    string description;
  };
  
  struct parsedCoord {
    string id;
    double x;
    double y;
  };
  
  
}

#pragma mark - Struct adaptors

BOOST_FUSION_ADAPT_STRUCT(
                          WaterObjects::parsedJunction,
                          (std::string, id)
                          (double, elevation)
                          (double, demand)
                          (std::string, pattern)
                          (std::string, description) );
BOOST_FUSION_ADAPT_STRUCT(
                          WaterObjects::parsedReservoir,
                          (string, id)
                          (double, head)
                          (string, pattern)
                          (string, description) );
BOOST_FUSION_ADAPT_STRUCT(
                          WaterObjects::parsedTank,
                          (string, id)
                          (double, elevation)
                          (double, initLevel)
                          (double, minLevel)
                          (double, maxLevel)
                          (double, diameter)
                          (double, minVol)
                          (string, volumeCurve)
                          (string, description) );

BOOST_FUSION_ADAPT_STRUCT(
                          WaterObjects::parsedPipe,
                          (string, id)
                          (string, node1)
                          (string, node2)
                          (double, length)
                          (double, diameter)
                          (double, roughness)
                          (double, minorLoss)
                          (string, status)
                          (string, description) );
BOOST_FUSION_ADAPT_STRUCT(
                          WaterObjects::parsedPump,
                          (string, id)
                          (string, node1)
                          (string, node2)
                          (string, parameters)
                          (string, description) );
BOOST_FUSION_ADAPT_STRUCT(
                          WaterObjects::parsedValve,
                          (string, id)
                          (string, node1)
                          (string, node2)
                          (double, diameter)
                          (string, type)
                          (string, info)
                          (string, description) );

// used for both coordinates and vertices
BOOST_FUSION_ADAPT_STRUCT(
                          WaterObjects::parsedCoord,
                          (string, id)
                          (double, x)
                          (double, y) );




#pragma mark - Qi Parsers & typedefs


namespace WaterObjects {
  
  // there's a difference between syntactical complexity and architectural complexity.
  
#pragma mark Junctions
  template <typename Iterator>
  struct junction_parser_t : grammar<Iterator, parsedJunction(), space_type> {
    junction_parser_t() : junction_parser_t::base_type(start) {
      wordLex %= lexeme[+char_(WORDLEX_STR)];
      commentLex %= -lit(';') >> lexeme[*(char_ - eol)]; // keep spaces
      //format: ID        Elevation   Demand    [Pattern]   [;Description]
      start %= wordLex >> double_ >> double_ >> -wordLex >> -commentLex;
      //BOOST_SPIRIT_DEBUG_NODE(start);
      //BOOST_SPIRIT_DEBUG_NODE(wordLex);
    }
    rule<Iterator, string(), space_type> wordLex, commentLex;
    rule<Iterator, parsedJunction(), space_type> start;
  };
  typedef junction_parser_t<std::string::iterator> junction_parser;

  template <typename Iterator>
  struct reservoir_parser_t : grammar<Iterator, parsedReservoir(), space_type> {
    reservoir_parser_t() : reservoir_parser_t::base_type(start) {
      wordLex %= lexeme[+char_(WORDLEX_STR)];
      commentLex %= -lit(';') >> lexeme[*(char_ - eol)]; // keep spaces
      //format: ID         Head      [Pattern]   [;Description]
      start %= wordLex >> double_ >> -wordLex >> -commentLex;
    }
    rule<Iterator, string(), space_type> wordLex, commentLex;
    rule<Iterator, parsedReservoir(), space_type> start;
  };
  typedef reservoir_parser_t<std::string::iterator> reservoir_parser;
  
  template <typename Iterator>
  struct tank_parser_t : grammar<Iterator, parsedTank(), space_type> {
    tank_parser_t() : tank_parser_t::base_type(start) {
      wordLex %= lexeme[+char_(WORDLEX_STR)];
      commentLex %= -lit(';') >> lexeme[*(char_ - eol)]; // keep spaces
      //format:   ID     Elevation  InitLevel  MinLevel   MaxLevel   Diameter   MinVol     [VolCurve]  [;Description]
      start %= wordLex >> double_ >> double_ >> double_ >> double_ >> double_ >> double_ >> -wordLex >> -commentLex;
    }
    rule<Iterator, string(), space_type> wordLex, commentLex;
    rule<Iterator, parsedTank(), space_type> start;
  };
  typedef tank_parser_t<std::string::iterator> tank_parser;
  
  
  
#pragma mark Pipes
  template <typename Iterator>
  struct pipe_parser_t : qi::grammar<Iterator, parsedPipe(), ascii::space_type> {
    pipe_parser_t() : pipe_parser_t::base_type(start) {
      wordLex %= lexeme[+char_("WORDLEX_STR")];
      commentLex %= -lit(';') >> lexeme[*(char_ - eol)]; // keep spaces
      start %= wordLex >> wordLex >> wordLex >> double_ >> double_ >> double_ >> double_ >> wordLex >> -commentLex;
      // BOOST_SPIRIT_DEBUG_NODE(descriptionLex) BOOST_SPIRIT_DEBUG_NODE(start);
    }
    rule<Iterator, string(), space_type> wordLex, commentLex;
    rule<Iterator, parsedPipe(), space_type> start;
  };
  typedef pipe_parser_t<std::string::iterator> pipe_parser;
  
  template <typename Iterator>
  struct pump_parser_t : qi::grammar<Iterator, parsedPump(), ascii::space_type> {
    pump_parser_t() : pump_parser_t::base_type(start) {
      wordLex %= lexeme[+char_("WORDLEX_STR")];
      commentLex %= -lit(';') >> lexeme[*(char_ - eol)]; // keep spaces
      // format: ID       Node1       Node2    Parameters   [;Description]
      start %= wordLex >> wordLex >> wordLex >> wordLex >> -commentLex; // TODO -- repeated parameter statements
      // BOOST_SPIRIT_DEBUG_NODE(descriptionLex) BOOST_SPIRIT_DEBUG_NODE(start);
    }
    rule<Iterator, string(), space_type> wordLex, commentLex;
    rule<Iterator, parsedPump(), space_type> start;
  };
  typedef pump_parser_t<std::string::iterator> pump_parser;
  
  template <typename Iterator>
  struct valve_parser_t : qi::grammar<Iterator, parsedValve(), ascii::space_type> {
    valve_parser_t() : valve_parser_t::base_type(start) {
      wordLex %= lexeme[+char_("WORDLEX_STR")];
      commentLex %= -lit(';') >> lexeme[*(char_ - eol)]; // keep spaces
      // format: ID       Node1      Node2      Diameter   Type	      Setting   MinorLoss   [;Description]
      start %= wordLex >> wordLex >> wordLex >> double_ >> wordLex >> wordLex >> double_ >> -commentLex;
      // BOOST_SPIRIT_DEBUG_NODE(descriptionLex) BOOST_SPIRIT_DEBUG_NODE(start);
    }
    rule<Iterator, string(), space_type> wordLex, commentLex;
    rule<Iterator, parsedValve(), space_type> start;
  };
  typedef valve_parser_t<std::string::iterator> valve_parser;
  
  
  
#pragma mark Coordinates
  template <typename Iterator>
  struct coord_parser_t : qi::grammar<Iterator, parsedCoord(), ascii::space_type> {
    coord_parser_t() : coord_parser_t::base_type(start) {
      wordLex %= lexeme[+char_("WORDLEX_STR")];
      start %= wordLex >> double_ >> double_ ;
      // BOOST_SPIRIT_DEBUG_NODE(start);
    }
    rule<Iterator, string(), space_type> wordLex;
    rule<Iterator, parsedCoord(), space_type> start;
  };
  typedef coord_parser_t<std::string::iterator> coord_parser;
  
  
}





#endif
