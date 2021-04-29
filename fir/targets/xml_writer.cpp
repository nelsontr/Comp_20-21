#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

static std::string qualifier_name(int qualifier) {
    if (qualifier == 1) return "public";
    else if (qualifier == 0) return "private";
    else if (qualifier == 2) return "import";
    else return "unknown qualifier";
}

static std::string newLine(bool nLine) {
    return nLine ? "Yes": "No";
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void fir::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void fir::xml_writer::do_double_node(cdk::double_node *const node, int lvl) {
    process_literal(node, lvl);
}

void fir::xml_writer::do_not_node(cdk::not_node *const node, int lvl) {
    do_unary_operation(node, lvl);
}

void fir::xml_writer::do_and_node(cdk::and_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_or_node(cdk::or_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_sequence_node(cdk::sequence_node *const node, int lvl) {
    os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
    for (size_t i = 0; i < node->size(); i++)
        if (node->node(i)) node->node(i)->accept(this, lvl + 2);
    closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_integer_node(cdk::integer_node *const node, int lvl) {
    process_literal(node, lvl);
}

void fir::xml_writer::do_string_node(cdk::string_node *const node, int lvl) {
    process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_unary_operation(cdk::unary_operation_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    node->argument()->accept(this, lvl + 2);
    closeTag(node, lvl);
}

void fir::xml_writer::do_neg_node(cdk::neg_node *const node, int lvl) {
    do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_binary_operation(cdk::binary_operation_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    node->left()->accept(this, lvl + 2);
    node->right()->accept(this, lvl + 2);
    closeTag(node, lvl);
}

void fir::xml_writer::do_add_node(cdk::add_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_sub_node(cdk::sub_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_mul_node(cdk::mul_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_div_node(cdk::div_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_mod_node(cdk::mod_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_lt_node(cdk::lt_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_le_node(cdk::le_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_ge_node(cdk::ge_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_gt_node(cdk::gt_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_ne_node(cdk::ne_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

void fir::xml_writer::do_eq_node(cdk::eq_node *const node, int lvl) {
    do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_alloc_node(fir::alloc_node *const node, int lvl) {
    do_unary_operation(node, lvl);
}

void fir::xml_writer::do_variable_node(cdk::variable_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">"
         << std::endl;
}

void fir::xml_writer::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    node->lvalue()->accept(this, lvl + 4);
    closeTag(node, lvl);
}

void fir::xml_writer::do_assignment_node(cdk::assignment_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);

    node->lvalue()->accept(this, lvl);
    reset_new_symbol();

    node->rvalue()->accept(this, lvl + 4);
    closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_evaluation_node(fir::evaluation_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    node->argument()->accept(this, lvl + 2);
    closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_read_node(fir::read_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    node->argument()->accept(this, lvl + 2);
    closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_while_node(fir::while_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);

    openTag("condition", lvl + 2);
    node->condition()->accept(this, lvl + 4);
    closeTag("condition", lvl + 2);

    openTag("block", lvl + 2);
    node->block()->accept(this, lvl + 4);
    closeTag("block", lvl + 2);

    closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void fir::xml_writer::do_if_node(fir::if_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);

    openTag("condition", lvl + 2);
    node->condition()->accept(this, lvl + 4);
    closeTag("condition", lvl + 2);

    openTag("then", lvl + 2);
    node->block()->accept(this, lvl + 4);
    closeTag("then", lvl + 2);

    closeTag(node, lvl);
}

void fir::xml_writer::do_if_else_node(fir::if_else_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);

    openTag("condition", lvl + 2);
    node->condition()->accept(this, lvl + 4);
    closeTag("condition", lvl + 2);

    openTag("then", lvl + 2);
    node->thenblock()->accept(this, lvl + 4);
    closeTag("then", lvl + 2);

    openTag("else", lvl + 2);
    node->elseblock()->accept(this, lvl + 4);
    closeTag("else", lvl + 2);

    closeTag(node, lvl);
}

void fir::xml_writer::do_return_node(fir::return_node *const node, int lvl) {
    //ASSERT_SAFE;
    openTag(node, lvl);
    if (node->value()) node->value()->accept(this, lvl + 4);
    closeTag(node, lvl);
}

void fir::xml_writer::do_leave_node(fir::leave_node *const node, int lvl) {
    openTag(node, lvl);
    closeTag(node, lvl);
}

void fir::xml_writer::do_while_finally_node(fir::while_finally_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);

    openTag("condition", lvl + 2);
    node->condition()->accept(this, lvl + 4);
    closeTag("condition", lvl + 2);

    openTag("block", lvl + 2);
    node->block()->accept(this, lvl + 4);
    closeTag("block", lvl + 2);

    if (node->finally_block()) {
        openTag("finally_block", lvl + 2);
        node->finally_block()->accept(this, lvl + 4);
        closeTag("finally_block", lvl + 2);
    }

    closeTag(node, lvl);
}

void fir::xml_writer::do_restart_node(fir::restart_node *const node, int lvl) {
    openTag(node, lvl);
    closeTag(node, lvl);
}

void fir::xml_writer::do_declaration_variable_node(fir::declaration_variable_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    reset_new_symbol();

    os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->variableId() << "' qualifier='"
         << qualifier_name(node->qualifier()) << "' type='" << cdk::to_string(node->type()) << "'>" << std::endl;

    if (node->initializer()) {
        openTag("initializer", lvl + 2);
        node->initializer()->accept(this, lvl + 4);
        closeTag("initializer", lvl + 2);
    }

    closeTag(node, lvl);
}

void fir::xml_writer::do_function_call_node(fir::function_call_node *const node, int lvl) {
    os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "'>" << std::endl;

    if (node->arguments()) {
        openTag("arguments", lvl);
        node->arguments()->accept(this, lvl + 4);
        closeTag("arguments", lvl);
    }

    closeTag(node, lvl);
}

void fir::xml_writer::do_function_declaration_node(fir::function_declaration_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSION;
    reset_new_symbol();

    //ERROR??

    os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "' qualifier='"
         << qualifier_name(node->qualifier()) << "' type='" << cdk::to_string(node->type()) << "'>" << std::endl;

    openTag("arguments", lvl);
    if (node->arguments()) {
        _symtab.push();
        node->arguments()->accept(this, lvl + 4);
        _symtab.pop();
    }
    closeTag("arguments", lvl);
    closeTag(node, lvl);
}

void fir::xml_writer::do_function_definition_node(fir::function_definition_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    reset_new_symbol();
    if (node->block()) _symtab.push();

    os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "' qualifier='"
         << qualifier_name(node->qualifier()) << "' type='" << cdk::to_string(node->type()) << "'>" << std::endl;

    if (node->arguments()) {
        openTag("arguments", lvl + 2);
        if (node->block()) node->arguments()->accept(this, lvl + 4);
        else {
            _symtab.push();
            node->arguments()->accept(this, lvl + 4);
            _symtab.pop();
        }
        closeTag("arguments", lvl + 2);
    }

    if (node->block()) {
        openTag("block", lvl + 2);
        node->block()->accept(this, lvl + 4);
        closeTag("block", lvl + 2);
    }

    closeTag(node, lvl);
    if (node->block()) _symtab.pop();
}

void fir::xml_writer::do_identify_node(fir::identify_node *const node, int lvl) {
    do_unary_operation(node, lvl);
}

void fir::xml_writer::do_null_pointer_node(fir::null_pointer_node *const node, int lvl) {
    openTag(node, lvl);
    closeTag(node, lvl);
}

void fir::xml_writer::do_pointer_node(fir::pointer_node *const node, int lvl) {
    openTag(node, lvl);
    if (node->base()) node->base()->accept(this, lvl + 2);
    if (node->index()) node->index()->accept(this, lvl + 2);
    closeTag(node, lvl);
}

void fir::xml_writer::do_size_of_node(fir::size_of_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    if (node->statement()) node->statement()->accept(this, lvl + 2);
    closeTag(node, lvl);
}

void fir::xml_writer::do_address_of_node(fir::address_of_node *const node, int lvl) {
    openTag(node, lvl);
    if (node->lvalue()) node->lvalue()->accept(this, lvl + 2);
    closeTag(node, lvl);
}

void fir::xml_writer::do_block_node(fir::block_node *const node, int lvl) {
    //ASSERT_SAFE_EXPRESSIONS;
    openTag(node, lvl);
    if (node->declarations()) {
        openTag("declarations", lvl);
        node->declarations()->accept(this, lvl + 4);
        closeTag("declarations", lvl);
    }
    if (node->instructions()) {
        openTag("instructions", lvl);
        node->instructions()->accept(this, lvl + 4);
        closeTag("instructions", lvl);
    }
    if (node->type()) {
        os() << std::string(lvl, ' ') << "<" << node->label()<<"_type" << ">" 
            << qualifier_name(node->type()) << "</" << node->label()<<"_type" << ">" << std::endl;
    }
    closeTag(node, lvl);
}



void fir::xml_writer::do_write_node(fir::write_node *const node, int lvl) {
    //ASSERT_SAFE;
    openTag(node, lvl);
    if (node->nLine()) {
        os() << std::string(lvl, ' ') << "<" << node->label()<<"_nLine" << ">" 
            << newLine(node->nLine()) << "</" << node->label()<<"_nLine" << ">" << std::endl;
    }
    if (node->argument()) node->argument()->accept(this, lvl + 2);
    closeTag(node, lvl);
}