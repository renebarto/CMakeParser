#include "cmake-parser/ScriptParser.h"

#include <fstream>
#include "serialization/EnumSerialization.h"
#include "utility/CommandLine.h"
#include "utility/StringFunctions.h"
#include "tracing/Tracing.h"
#include "cmake-parser/Expression.h"
#include "cmake-parser/ParserExceptions.h"

using namespace utility;
using namespace parser;
using namespace std::placeholders;

namespace cmake_parser {

using Data = Token<Terminal>;

enum class Keyword {
    AddExecutable,
    AddLibrary,
    AddSubDirectory,
    CMakeMinimumRequired,
    Else,
    EndForEach,
    EndFunction,
    EndIf,
    EndMacro,
    FindPackage,
    Force,
    ForEach,
    Function,
    GetCMakeProperty,
    GetTargetProperties,
    If,
    Include,
    LinkDirectories,
    List,
    Macro,
    Message,
    Option,
    Project,
    Set,
    SetProperty,
    SetTargetProperties,
    String,
    TargetCompileDefinitions,
    TargetCompileOptions,
    TargetIncludeDirectories,
    TargetLinkLibraries,
    Unset,
};

} // cmake_parser

namespace serialization {

template<>
const BidirectionalMap<cmake_parser::Keyword, std::string> EnumSerializationMap<cmake_parser::Keyword>::ConversionMap =
{
    { cmake_parser::Keyword::AddExecutable, "add_executable" },
    { cmake_parser::Keyword::AddLibrary, "add_library" },
    { cmake_parser::Keyword::AddSubDirectory, "add_subdirectory" },
    { cmake_parser::Keyword::CMakeMinimumRequired, "cmake_minimum_required" },
    { cmake_parser::Keyword::Else, "else" },
    { cmake_parser::Keyword::EndForEach, "endforeach" },
    { cmake_parser::Keyword::EndFunction, "endfunction" },
    { cmake_parser::Keyword::EndIf, "endif" },
    { cmake_parser::Keyword::EndMacro, "endmacro" },
    { cmake_parser::Keyword::FindPackage, "find_package" },
    { cmake_parser::Keyword::Force, "force" },
    { cmake_parser::Keyword::ForEach, "foreach" },
    { cmake_parser::Keyword::Function, "function" },
    { cmake_parser::Keyword::GetCMakeProperty, "get_cmake_property" },
    { cmake_parser::Keyword::GetTargetProperties, "get_target_properties" },
    { cmake_parser::Keyword::If, "if" },
    { cmake_parser::Keyword::Include, "include" },
    { cmake_parser::Keyword::LinkDirectories, "link_directories" },
    { cmake_parser::Keyword::List, "list" },
    { cmake_parser::Keyword::Macro, "macro" },
    { cmake_parser::Keyword::Message, "message" },
    { cmake_parser::Keyword::Option, "option" },
    { cmake_parser::Keyword::Project, "project" },
    { cmake_parser::Keyword::Set, "set" },
    { cmake_parser::Keyword::SetProperty, "set_property" },
    { cmake_parser::Keyword::SetTargetProperties, "set_target_properties" },
    { cmake_parser::Keyword::String, "string" },
    { cmake_parser::Keyword::TargetCompileDefinitions, "target_compile_definitions" },
    { cmake_parser::Keyword::TargetCompileOptions, "target_compile_options" },
    { cmake_parser::Keyword::TargetIncludeDirectories, "target_include_directories" },
    { cmake_parser::Keyword::TargetLinkLibraries, "target_link_libraries" },
    { cmake_parser::Keyword::Unset, "unset" },
};

} // namespace serialization

namespace cmake_parser {

inline std::ostream& operator << (std::ostream& stream, Keyword keyword)
{
    return stream << serialization::Serialize(keyword);
}

static bool LookupKeyword(const Token<Terminal>& token, Keyword& keyword)
{
    for (auto const& item : serialization::EnumSerializationMap<Keyword>::GetValues())
    {
        if (utility::IsEqualIgnoreCase(item, token.Value()))
        {
            serialization::Deserialize(item, keyword);
            return true;
        }
    }
    return false;
}

static void PrintToken(const Token<Terminal>& token)
{
    TRACE_DEBUG("Token: {}", token);
}

static void PrintUngetToken(const Token<Terminal>& token)
{
    TRACE_DEBUG("Unget: {}", token);
}

static void PrintTokenSkip(const Token<Terminal>& token)
{
    TRACE_DEBUG("Skip: {}", token);
}

ScriptParser::ScriptParser(CMakeModel& model, const std::filesystem::path& rootDirectory, std::istream& stream)
    : m_path{ rootDirectory / CMakeScriptFileName }
    , m_lexer{ m_path, stream }
    , m_currentToken{}
    , m_errorCount{}
    , m_model{ model }
    , m_mainProject{}
    , m_currentProject{}
{
}

bool ScriptParser::Parse()
{
    TRACE_INFO("Start parsing {}", m_path.generic_string());
    ParserExecutor<Terminal> parserExecutor(*this, { Terminal::Whitespace, Terminal::NewLine });

    auto result = parserExecutor.Parse(m_lexer) && (m_errorCount == 0);
    TRACE_INFO("End parsing {}: result {}, errors {}", m_path.generic_string(), result, m_errorCount);
    return result;
}

std::string ScriptParser::ParseVersion(const TerminalSet& endTerminals)
{
    std::string version;

    try
    {
        NextToken();
        SkipWhitespace();
        auto expressionText = ExpectExpression(endTerminals);
        expression::Expression expr(m_model, expressionText);
        version = expr.Evaluate();
    }
    catch (std::exception&)
    { 
        return {};
    }

    return version;
}

std::string ScriptParser::ParseDescription()
{
    std::string description;

    try
    {
        NextToken();
        SkipWhitespace();
        description = utility::UnQuote(Expect(Terminal::String));
    }
    catch (std::exception&)
    {
        return {};
    }

    return description;
}

std::string ScriptParser::ParseLanguages(const TerminalSet& endTerminals)
{
    std::string languages;

    try
    {
        NextToken();
        SkipWhitespace();
        languages = Expect(Terminal::Identifier);
        SkipWhitespace();
        while (!CurrentTokenInSet(endTerminals))
        {
            languages += " " + Expect(Terminal::Identifier);
            SkipWhitespace();
        }
        UngetCurrentToken();
    }
    catch (std::exception&)
    {
        return {};
    }

    return languages;
}

void ScriptParser::NextToken()
{
    m_currentToken = m_lexer.GetToken();
    PrintToken(CurrentToken());
}

Terminal ScriptParser::CurrentTokenType()
{
    return CurrentToken().Type().TypeCode();
}

const parser::Token<Terminal>& ScriptParser::CurrentToken()
{
    return m_currentToken;
}

bool ScriptParser::CurrentTokenInSet(const TerminalSet& terminals)
{
    return terminals.find(CurrentTokenType()) != terminals.end();
}

void ScriptParser::UngetCurrentToken()
{
    m_lexer.UngetToken(CurrentToken());
    PrintUngetToken(CurrentToken());
}

void ScriptParser::SkipWhitespace()
{
    while ((CurrentTokenType() == Terminal::Whitespace) || (CurrentTokenType() == Terminal::NewLine))
    {
        NextToken();
    }
}

std::string ScriptParser::Expect(Terminal type)
{
    if (CurrentTokenType() != type)
    {
        OnParseError(CurrentToken());
        throw UnexpectedToken(CurrentToken(), __FILE__, __LINE__);
    }
    auto result = CurrentToken().Value();
    NextToken();
    return result;
}

std::string ScriptParser::Expect_SkipWhitespace(Terminal type)
{
    SkipWhitespace();
    return Expect(type);
}

Token<Terminal> ScriptParser::Expect(std::set<Terminal> oneOfTypes)
{
    if (oneOfTypes.find(CurrentTokenType()) == oneOfTypes.end())
    {
        OnParseError(CurrentToken());
        throw UnexpectedToken(CurrentToken(), __FILE__, __LINE__);
    }
    auto result = CurrentToken();
    NextToken();
    return result;
}

Token<Terminal> ScriptParser::Expect_SkipWhitespace(std::set<Terminal> oneOfTypes)
{
    SkipWhitespace();
    return Expect(oneOfTypes);
}

std::string ScriptParser::ExpectVariable()
{
    std::string result;

    Expect(Terminal::CurlyBraceOpen);
    result = Expect(Terminal::Identifier);
    Expect(Terminal::CurlyBraceClose);

    return result;
}

std::filesystem::path ScriptParser::ExpectPath(const std::set<Terminal>& finalizers)
{
    auto expression = ExpectExpression(finalizers);
    std::filesystem::path path = m_model.GetVariable(VarCurrentSourceDirectory);
    path = std::filesystem::canonical(path / expression);
    if (!std::filesystem::exists(path))
    {
        throw UnexpectedPath(expression, __FILE__, __LINE__);
    }
    return path;
}

std::string ScriptParser::ExpectExpression(const std::set<Terminal>& finalizers)
{
    std::string value{};

    while (finalizers.find(CurrentToken().Type().TypeCode()) == finalizers.end())
    {
        value += ExpectExpressionPart();
    }
    return value;
}

std::string ScriptParser::ExpectExpressionPart()
{
    std::string result;

    switch (CurrentToken().Type().TypeCode())
    {
    case Terminal::Dollar:
        result = Expect(Terminal::Dollar);
        result += Expect(Terminal::CurlyBraceOpen);
        result += Expect(Terminal::Identifier);
        result += Expect(Terminal::CurlyBraceClose);
        return result;
    default:
        break;
    }

    result = CurrentToken().Value();
    NextToken();
    return result;
}

std::string ScriptParser::Evaluate(const std::string& expression) const
{
    expression::Expression expr(GetModel(), expression);
    return expr.Evaluate();
}

bool ScriptParser::HandleCMakeMinimumRequired()
{
    // Attributes
    static const std::string VersionKeyword{ "version" };

    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    std::string str = Expect_SkipWhitespace(Terminal::Identifier);
    if (!utility::IsEqualIgnoreCase(VersionKeyword, str))
        return false;
    auto result = ParseVersion({ Terminal::Whitespace, Terminal::NewLine, Terminal::ParenthesisClose });
    if (result.empty())
        return false;
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    m_model.SetVariable(VarMinimumRequiredVersion, result);
    return true;
}

bool ScriptParser::HandleProject()
{
    // Attributes
    static const std::string VersionKeyword{ "version" };
    static const std::string DescriptionKeyword{ "description" };
    static const std::string LanguagesKeyword{ "languages" };

    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    auto projectName = Expect_SkipWhitespace({ Terminal::Name, Terminal::Identifier });
    auto result = projectName.Value();
    m_currentProject = std::make_shared<Project>(result);
    SkipWhitespace();
    while (CurrentTokenType() == Terminal::Identifier)
    {
        result = Expect(Terminal::Identifier);
        if (utility::IsEqualIgnoreCase(VersionKeyword, result))
        {
            result = ParseVersion({ Terminal::Whitespace, Terminal::NewLine, Terminal::ParenthesisClose });
            if (!result.empty())
            {
                m_currentProject->SetVersion(result);
            }
        }
        else if (utility::IsEqualIgnoreCase(DescriptionKeyword, result))
        {
            result = ParseDescription();
            if (!result.empty())
            {
                m_currentProject->SetDescription(result);
            }
        }
        else if (utility::IsEqualIgnoreCase(LanguagesKeyword, result))
        {
            result = ParseLanguages({ Terminal::ParenthesisClose });
            if (!result.empty())
            {
                m_currentProject->SetLanguages(result);
            }
        }
        else
        {
            return false;
        }
        SkipWhitespace();
    }
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    m_model.AddProject(m_currentProject);
    return true;
}

// Message types
static const std::string MessageModeFatalError{ "FATAL_ERROR" };
static const std::string MessageModeSendError{ "SEND_ERROR" };
static const std::string MessageModeWarning{ "WARNING" };
static const std::string MessageModeAuthorWarning{ "AUTHOR_WARNING" };
static const std::string MessageModeDeprecation{ "DEPRECATION" };
static const std::string MessageModeNotice{ "NOTICE" };
static const std::string MessageModeStatus{ "STATUS" };
static const std::string MessageModeVerbose{ "VERBOSE" };
static const std::string MessageModeDebug{ "DEBUG" };
static const std::string MessageModeTrace{ "TRACE" };

bool ScriptParser::HandleMessage()
{
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    auto token = Expect_SkipWhitespace({ Terminal::Identifier, Terminal::String });
    std::string messageMode;
    std::string message;
    if (token.Type() == Terminal::Identifier)
    {
        if (!IsValidMessageMode(token.Value()))
        {
            return false;
        }
        messageMode = token.Value();
        message = Expect_SkipWhitespace(Terminal::String);
    }
    else
    {
        messageMode = MessageModeNotice;
        message = token.Value();
    }
    m_model.AddMessage(messageMode, message);
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    return true;
}

bool ScriptParser::IsValidMessageMode(const std::string& mode)
{
    return
        IsEqualIgnoreCase(MessageModeFatalError, mode) ||
        IsEqualIgnoreCase(MessageModeSendError, mode) ||
        IsEqualIgnoreCase(MessageModeWarning, mode) ||
        IsEqualIgnoreCase(MessageModeAuthorWarning, mode) ||
        IsEqualIgnoreCase(MessageModeDeprecation, mode) ||
        IsEqualIgnoreCase(MessageModeNotice, mode) ||
        IsEqualIgnoreCase(MessageModeStatus, mode) ||
        IsEqualIgnoreCase(MessageModeVerbose, mode) ||
        IsEqualIgnoreCase(MessageModeDebug, mode) ||
        IsEqualIgnoreCase(MessageModeTrace, mode);
}

// Variable attributes
static const std::string EnvironmentPrefix{ "ENV" };
static const std::string KeywordCache{ "CACHE" };
static const std::string KeywordParentScope{ "PARENT_SCOPE" };
static const std::string KeywordForce{ "FORCE" };

bool ScriptParser::HandleSet()
{
    std::set<Terminal> finalizers = { Terminal::Whitespace , Terminal::NewLine, Terminal::ParenthesisClose };

    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    SkipWhitespace();
    if ((CurrentToken().Type() == Terminal::Identifier) && (IsEqualIgnoreCase(EnvironmentPrefix, CurrentToken().Value())))
    {
        Expect(Terminal::Identifier);
        std::string variableName = ExpectVariable();
        SkipWhitespace();
        std::string value = ExpectExpression(finalizers);
        if (value.empty())
        {
            m_model.UnsetEnvironmentVariable(variableName);
        }
        else
        {
            m_model.SetEnvironmentVariable(variableName, value);
        }
    }
    else
    {
        if (CurrentToken().Type() == Terminal::Identifier)
        {
            std::string variableName = ExpectExpression(finalizers);
            std::string variableType{};
            std::string description{};
            std::string value{};
            VariableAttribute variableAttributes{};
            SkipWhitespace();
            while (CurrentToken().Type() != Terminal::ParenthesisClose)
            {
                if (CurrentToken().Type() == Terminal::Identifier)
                {
                    if (IsEqualIgnoreCase(KeywordCache, CurrentToken().Value()))
                    {
                        Expect(Terminal::Identifier);
                        variableAttributes = variableAttributes | VariableAttribute::Cache;
                        variableType = Expect_SkipWhitespace(Terminal::Identifier);
                        description = Expect_SkipWhitespace(Terminal::String);
                        SkipWhitespace();
                        if (CurrentToken().Type() != Terminal::ParenthesisClose)
                        {
                            if (IsEqualIgnoreCase(KeywordForce, CurrentToken().Value()))
                            {
                                Expect(Terminal::Identifier);
                                variableAttributes = variableAttributes | VariableAttribute::Force;
                            }
                            else
                                throw UnexpectedToken(CurrentToken(), __FILE__, __LINE__);
                        }
                        break;
                    }
                    else if (IsEqualIgnoreCase(KeywordParentScope, CurrentToken().Value()))
                    {
                        variableAttributes = variableAttributes | VariableAttribute::ParentScope;
                        break;
                    }
                }
                if (!value.empty())
                    value += ";";

                value += ExpectExpression(finalizers);
                SkipWhitespace();
            }
            if (value.empty())
            {
                m_model.UnsetVariable(variableName, variableAttributes);
            }
            else
            {
                m_model.SetVariable(variableName, value, variableAttributes, variableType, description);
            }
        }
        else
        {
            throw UnexpectedToken(CurrentToken(), __FILE__, __LINE__);
        }
    }
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    return true;
}

bool ScriptParser::HandleUnset()
{
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    SkipWhitespace();
    if ((CurrentToken().Type() == Terminal::Identifier) && (IsEqualIgnoreCase(EnvironmentPrefix, CurrentToken().Value())))
    {
        Expect(Terminal::Identifier);
        std::string variableName = ExpectVariable();
        m_model.UnsetEnvironmentVariable(variableName);
    }
    else
    {
        if (CurrentToken().Type() == Terminal::Identifier)
        {
            std::string variableName = Expect(Terminal::Identifier);
            VariableAttribute variableAttributes{};
            SkipWhitespace();
            if (CurrentToken().Type() == Terminal::Identifier)
            {
                if (IsEqualIgnoreCase(KeywordCache, CurrentToken().Value()))
                {
                    Expect(Terminal::Identifier);
                    variableAttributes = variableAttributes | VariableAttribute::Cache;
                    SkipWhitespace();
                }
                else if (IsEqualIgnoreCase(KeywordParentScope, CurrentToken().Value()))
                {
                    variableAttributes = variableAttributes | VariableAttribute::ParentScope;
                }
                m_model.UnsetVariable(variableName, variableAttributes);
            }
        }
    }
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    return true;
}

bool ScriptParser::HandleAddSubdirectory()
{
    std::set<Terminal> finalizers = { Terminal::Whitespace , Terminal::NewLine, Terminal::ParenthesisClose };

    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    auto path = ExpectPath(finalizers);
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    m_model.EnterDirectory(std::filesystem::relative(path, m_model.GetCurrentDirectory()->SourcePath()).generic_string());

    std::ifstream stream(path / CMakeScriptFileName);
    ScriptParser parser(m_model, path, stream);

    auto result = parser.Parse();

    m_model.LeaveDirectory();

    return result;
}

bool ScriptParser::HandleUnsupported()
{
    Expect_SkipWhitespace(Terminal::ParenthesisOpen);
    do
    {
        SkipWhitespace();
        if (CurrentToken().Type() == Terminal::ParenthesisOpen)
            HandleUnsupported();
        else if (CurrentToken().Type() != Terminal::ParenthesisClose)
            NextToken();
    } while (CurrentToken().Type() != Terminal::ParenthesisClose);
    Expect_SkipWhitespace(Terminal::ParenthesisClose);
    return true;
}

bool ScriptParser::OnToken(const parser::Token<Terminal>& token, bool& done)
{
    PrintToken(token);
    NextToken();
    Keyword keyword;
    bool result{};
    if (!LookupKeyword(token, keyword))
    {
        if (token.Type() == Terminal::Comment)
            return true;
        TRACE_WARNING("Skipping unknown token {}", token.Value());
        return HandleUnsupported();
    }
    switch (keyword)
    {
    case Keyword::CMakeMinimumRequired:
        result = HandleCMakeMinimumRequired();
        break;
    case Keyword::Project:
        result = HandleProject();
        break;
    case Keyword::Message:
        result = HandleMessage();
        break;
    case Keyword::Set:
        result = HandleSet();
        break;
    case Keyword::Unset:
        result = HandleUnset();
        break;
    case Keyword::AddSubDirectory:
        result = HandleAddSubdirectory();
        break;
    case Keyword::AddExecutable:
    case Keyword::AddLibrary:
    case Keyword::Else:
    case Keyword::EndForEach:
    case Keyword::EndFunction:
    case Keyword::EndIf:
    case Keyword::EndMacro:
    case Keyword::FindPackage:
    case Keyword::ForEach:
    case Keyword::Function:
    case Keyword::GetCMakeProperty:
    case Keyword::GetTargetProperties:
    case Keyword::If:
    case Keyword::Include:
    case Keyword::LinkDirectories:
    case Keyword::List:
    case Keyword::Macro:
    case Keyword::Option:
    case Keyword::SetProperty:
    case Keyword::SetTargetProperties:
    case Keyword::String:
    case Keyword::TargetCompileDefinitions:
    case Keyword::TargetCompileOptions:
    case Keyword::TargetIncludeDirectories:
    case Keyword::TargetLinkLibraries:
        TRACE_WARNING("Skipping unsupported keyword {}", keyword);
        result = HandleUnsupported();
        break;
    default:
        break;
    }

    done = false;
    return result;
}

bool ScriptParser::OnNoMoreToken(const parser::SourceLocation& /*location*/)
{
    TRACE_INFO("No more tokens");
    return true;
}

void ScriptParser::OnSkipToken(const parser::Token<Terminal>& token)
{
    PrintTokenSkip(token);
}

void ScriptParser::OnParseError(const parser::Token<Terminal>& token)
{
    m_errorCount++;
    TRACE_ERROR("Parse error: unexpected token {}", token);
}

} // namespace cmake_parser
