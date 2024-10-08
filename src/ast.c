#include "include/ast.h"
#include <stdio.h>
#include <string.h>

// Function to initialize an AST node of a given type.
ast_ *init_ast(enum ast_type type)
{
  ast_ *ast = calloc(1, sizeof(struct AST_STRUCT));
  ast->type = type;

  // Initialize all fields to default "null" state
  ast->scope = NULL;

  ast->int_value.null = 1;
  ast->int_value.value = 0;
  ast->real_value.null = 1;
  ast->real_value.value = 0.0;
  ast->char_value.null = 1;
  ast->char_value.value = '\0';
  ast->string_value = NULL;
  ast->boolean_value.null = 1;
  ast->boolean_value.value = 0;
  ast->array_elements = NULL;
  ast->array_size = 0;
  ast->array_dimension = 0;
  ast->array_type = AST_NOOP;

  ast->compound_value = NULL;
  ast->lhs = NULL;
  ast->rhs = NULL;
  ast->constant = 0;
  ast->userinput = 0;
  ast->variable_name = NULL;
  ast->field_name = NULL;
  ast->index = NULL;
  ast->class_name = NULL;
  ast->arguments = NULL;
  ast->arguments_count = 0;
  ast->left = NULL;
  ast->right = NULL;
  ast->op = NULL;
  ast->record_name = NULL;
  ast->record_elements = NULL;
  ast->field_count = 0;
  ast->subroutine_name = NULL;
  ast->parameters = NULL;
  ast->parameter_count = 0;
  ast->body = NULL;
  ast->return_value = NULL;
  ast->output_expressions = NULL;
  ast->loop_variable = NULL;
  ast->end_expr = NULL;
  ast->step_expr = NULL;
  ast->collection_expr = NULL;
  ast->condition = NULL;
  ast->indefinite_loop_type = 0;
  ast->loop_body = NULL;
  ast->if_condition = NULL;
  ast->if_body = NULL;
  ast->else_if_conditions = NULL;
  ast->else_if_bodies = NULL;
  ast->else_body = NULL;
  ast->exit_code = 0;

  return ast;
}

ast_ **init_ast_list()
{
  ast_ **list = malloc(sizeof(ast_ *));
  if (list == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed during AST list initialization.\n");
    return NULL;
  }
  list[0] = NULL;
  return list;
}

void add_ast_to_list(ast_ ***list, ast_ *new_ast)
{
  size_t count = 0;

  if (*list == NULL || new_ast == NULL)
  {
    fprintf(stderr, "Error: Cannot add AST to list. The list or AST is NULL. Ensure the list is initialized before adding elements.\n");
    return;
  }

  if (new_ast->type == AST_NOOP)
  {
    return;
  }

  while ((*list)[count] != NULL)
  {
    count++;
  }

  ast_ **temp = realloc(*list, sizeof(ast_ *) * (count + 2));
  if (temp == NULL)
  {
    fprintf(stderr, "Error: Memory reallocation failed while expanding AST list. Current list size: %zu elements.\n", count);
    return;
  }

  *list = temp;
  (*list)[count] = new_ast;
  (*list)[count + 1] = NULL;
}

ast_ *deep_copy(ast_ *original)
{
  if (original == NULL)
  {
    return NULL;
  }

  // Allocate memory for the new AST node
  ast_ *copy = init_ast(original->type);
  if (copy == NULL)
  {
    return NULL;
  }

  // Copy simple fields
  copy->int_value = original->int_value;
  copy->real_value = original->real_value;
  copy->char_value = original->char_value;
  copy->boolean_value = original->boolean_value;

  copy->constant = original->constant;
  copy->userinput = original->userinput;

  copy->array_size = original->array_size;
  copy->array_type = original->array_type;
  copy->array_dimension = original->array_dimension;
  copy->field_count = original->field_count;
  copy->parameter_count = original->parameter_count;
  copy->arguments_count = original->arguments_count;
  copy->exit_code = original->exit_code;

  // Deep copy of strings
  copy->string_value = original->string_value ? strdup(original->string_value) : NULL;
  copy->variable_name = original->variable_name ? strdup(original->variable_name) : NULL;
  copy->field_name = original->field_name ? strdup(original->field_name) : NULL;
  copy->class_name = original->class_name ? strdup(original->class_name) : NULL;
  copy->record_name = original->record_name ? strdup(original->record_name) : NULL;
  copy->op = original->op ? strdup(original->op) : NULL;
  copy->subroutine_name = original->subroutine_name ? strdup(original->subroutine_name) : NULL;

  // Deep copy of AST subtrees (lhs, rhs, left, right, condition, etc.)
  copy->lhs = deep_copy(original->lhs);
  copy->rhs = deep_copy(original->rhs);
  copy->left = deep_copy(original->left);
  copy->right = deep_copy(original->right);
  copy->return_value = deep_copy(original->return_value);
  copy->loop_variable = deep_copy(original->loop_variable);
  copy->end_expr = deep_copy(original->end_expr);
  copy->step_expr = deep_copy(original->step_expr);
  copy->collection_expr = deep_copy(original->collection_expr);
  copy->condition = deep_copy(original->condition);
  copy->if_condition = deep_copy(original->if_condition);

  // Deep copy of AST lists (compound_value, array_elements, arguments, etc.)
  if (original->compound_value)
  {
    size_t compound_size = 0;
    while (original->compound_value[compound_size] != NULL)
    {
      compound_size++;
    }

    copy->compound_value = malloc((compound_size + 1) * sizeof(ast_ *));
    for (size_t i = 0; i < compound_size; i++)
    {
      copy->compound_value[i] = deep_copy(original->compound_value[i]);
    }
    copy->compound_value[compound_size] = NULL;
  }

  if (original->array_elements)
  {
    copy->array_elements = malloc((original->array_size + 1) * sizeof(ast_ *));
    for (int i = 0; i < original->array_size; i++)
    {
      copy->array_elements[i] = deep_copy(original->array_elements[i]);
    }
    copy->array_elements[original->array_size] = NULL;
  }

  if (original->arguments)
  {
    copy->arguments = malloc((original->arguments_count) * sizeof(ast_ *));
    for (int i = 0; i < original->arguments_count; i++)
    {
      copy->arguments[i] = deep_copy(original->arguments[i]);
    }
  }

  if (original->parameters)
  {
    copy->parameters = malloc((original->parameter_count) * sizeof(ast_ *));
    for (int i = 0; i < original->parameter_count; i++)
    {
      copy->parameters[i] = deep_copy(original->parameters[i]);
    }
  }

  if (original->body)
  {
    size_t body_size = 0;
    while (original->body[body_size] != NULL)
    {
      body_size++;
    }

    copy->body = malloc((body_size + 1) * sizeof(ast_ *));
    for (size_t i = 0; i < body_size; i++)
    {
      copy->body[i] = deep_copy(original->body[i]);
    }
    copy->body[body_size] = NULL;
  }

  if (original->output_expressions)
  {
    size_t output_size = 0;
    while (original->output_expressions[output_size] != NULL)
    {
      output_size++;
    }

    copy->output_expressions = malloc((output_size + 1) * sizeof(ast_ *));
    for (size_t i = 0; i < output_size; i++)
    {
      copy->output_expressions[i] = deep_copy(original->output_expressions[i]);
    }
    copy->output_expressions[output_size] = NULL;
  }

  if (original->loop_body)
  {
    size_t loop_body_size = 0;
    while (original->loop_body[loop_body_size] != NULL)
    {
      loop_body_size++;
    }

    copy->loop_body = malloc((loop_body_size + 1) * sizeof(ast_ *));
    for (size_t i = 0; i < loop_body_size; i++)
    {
      copy->loop_body[i] = deep_copy(original->loop_body[i]);
    }
    copy->loop_body[loop_body_size] = NULL;
  }

  if (original->if_body)
  {
    size_t if_body_size = 0;
    while (original->if_body[if_body_size] != NULL)
    {
      if_body_size++;
    }

    copy->if_body = malloc((if_body_size + 1) * sizeof(ast_ *));
    for (size_t i = 0; i < if_body_size; i++)
    {
      copy->if_body[i] = deep_copy(original->if_body[i]);
    }
    copy->if_body[if_body_size] = NULL;
  }

  if (original->else_if_conditions)
  {
    size_t else_if_size = 0;
    while (original->else_if_conditions[else_if_size] != NULL)
    {
      else_if_size++;
    }

    copy->else_if_conditions = malloc((else_if_size + 1) * sizeof(ast_ *));
    for (size_t i = 0; i < else_if_size; i++)
    {
      copy->else_if_conditions[i] = deep_copy(original->else_if_conditions[i]);
    }
    copy->else_if_conditions[else_if_size] = NULL;
  }

  if (original->else_if_bodies)
  {
    size_t else_if_bodies_size = 0;
    while (original->else_if_bodies[else_if_bodies_size] != NULL)
    {
      else_if_bodies_size++;
    }

    copy->else_if_bodies = malloc((else_if_bodies_size + 1) * sizeof(ast_ **));
    for (size_t i = 0; i < else_if_bodies_size; i++)
    {
      size_t body_size = 0;
      while (original->else_if_bodies[i][body_size] != NULL)
      {
        body_size++;
      }

      copy->else_if_bodies[i] = malloc((body_size + 1) * sizeof(ast_ *));
      for (size_t j = 0; j < body_size; j++)
      {
        copy->else_if_bodies[i][j] = deep_copy(original->else_if_bodies[i][j]);
      }
      copy->else_if_bodies[i][body_size] = NULL;
    }
  }

  if (original->else_body)
  {
    size_t else_body_size = 0;
    while (original->else_body[else_body_size] != NULL)
    {
      else_body_size++;
    }

    copy->else_body = malloc((else_body_size + 1) * sizeof(ast_ *));
    for (size_t i = 0; i < else_body_size; i++)
    {
      copy->else_body[i] = deep_copy(original->else_body[i]);
    }
    copy->else_body[else_body_size] = NULL;
  }

  return copy;
}

// Function to convert an AST type to a string representation.
const char *ast_type_to_string(enum ast_type type)
{
  switch (type)
  {
  case AST_COMPOUND:
    return "AST_COMPOUND";
  case AST_NOOP:
    return "NOOP";
  case AST_INTEGER:
    return "AST_INTEGER";
  case AST_REAL:
    return "AST_REAL";
  case AST_CHARACTER:
    return "AST_CHARACTER";
  case AST_STRING:
    return "AST_STRING";
  case AST_BOOLEAN:
    return "AST_BOOLEAN";
  case AST_ARRAY:
    return "AST_ARRAY";
  case AST_RECORD:
    return "AST_RECORD";
  case AST_ASSIGNMENT:
    return "AST_ASSIGNMENT";
  case AST_VARIABLE:
    return "AST_VARIABLE";
  case AST_RECORD_ACCESS:
    return "AST_RECORD_ACCESS";
  case AST_ARRAY_ACCESS:
    return "AST_ARRAY_ACCESS";
  case AST_INSTANTIATION:
    return "AST_INSTANTIATION";
  case AST_ARITHMETIC_EXPRESSION:
    return "AST_ARITHMETIC_EXPRESSION";
  case AST_BOOLEAN_EXPRESSION:
    return "AST_BOOLEAN_EXPRESSION";
  case AST_RECORD_DEFINITION:
    return "AST_RECORD_DEFINITION";
  case AST_SUBROUTINE:
    return "AST_SUBROUTINE";
  case AST_RETURN:
    return "AST_RETURN";
  case AST_OUTPUT:
    return "AST_OUTPUT";
  case AST_DEFINITE_LOOP:
    return "AST_DEFINITE_LOOP";
  case AST_INDEFINITE_LOOP:
    return "AST_INDEFINITE_LOOP";
  case AST_SELECTION:
    return "AST_SELECTION";
  case AST_EXIT:
    return "AST_EXIT";
  default:
    return "AST_UNKNOWN";
  }
}

int valid(ast_ *ast)
{
  if (!ast)
    return 1; // Null check, assume valid if null

  switch (ast->type)
  {
  case AST_COMPOUND:
    if (ast->compound_value != NULL)
      return 0;
    break;

  case AST_INTEGER:
    if (ast->int_value.null == 0)
      return 0;
    break;

  case AST_REAL:
    if (ast->real_value.null == 0)
      return 0;
    break;

  case AST_CHARACTER:
    if (ast->char_value.null == 0)
      return 0;
    break;

  case AST_STRING:
    if (ast->string_value != NULL)
      return 0;
    break;

  case AST_BOOLEAN:
    if (ast->boolean_value.null == 0)
      return 0;
    break;

  case AST_ARRAY:
    if (ast->array_elements != NULL)
      return 0;
    break;

  case AST_ASSIGNMENT:
    if (ast->lhs != NULL || ast->rhs != NULL || ast->constant != 0 || ast->userinput != 0)
      return 0;
    break;

  case AST_VARIABLE:
    if (ast->variable_name != NULL)
      return 0;
    break;

  case AST_RECORD_ACCESS:
    if (ast->field_name != NULL)
      return 0;
    break;

  case AST_ARRAY_ACCESS:
    if (ast->index != NULL)
      return 0;
    break;

  case AST_INSTANTIATION:
    if (ast->class_name != NULL || ast->arguments != NULL)
      return 0;
    break;

  case AST_ARITHMETIC_EXPRESSION:
  case AST_BOOLEAN_EXPRESSION:
    if (ast->left != NULL || ast->right != NULL || ast->op != NULL)
      return 0;
    break;

  case AST_RECORD_DEFINITION:
  case AST_RECORD:
    if (ast->record_name != NULL || ast->record_elements != NULL)
      return 0;
    break;

  case AST_SUBROUTINE:
    if (ast->subroutine_name != NULL || ast->parameters != NULL || ast->body != NULL)
      return 0;
    break;

  case AST_RETURN:
    if (ast->return_value != NULL)
      return 0;
    break;

  case AST_OUTPUT:
    if (ast->output_expressions != NULL)
      return 0;
    break;

  case AST_DEFINITE_LOOP:
    if (ast->loop_variable != NULL || ast->end_expr != NULL || ast->step_expr != NULL || ast->collection_expr != NULL)
      return 0;
    break;

  case AST_INDEFINITE_LOOP:
    if (ast->condition != NULL || ast->loop_body != NULL)
      return 0;
    break;

  case AST_SELECTION:
    if (ast->if_condition != NULL || ast->if_body != NULL || ast->else_if_conditions != NULL || ast->else_if_bodies != NULL || ast->else_body != NULL)
      return 0;
    break;

  case AST_NOOP:
  default:
    break; // No-op remains unchanged
  }

  return 1; // All values match initial defaults
}

void print_indent(int indent)
{
  for (int i = 0; i < indent; i++)
  {
    printf("  ");
  }
}

void print_ast_literal(ast_ *node, int indent)
{
  switch (node->type)
  {
  case AST_INTEGER:
    print_indent(indent);
    if (node->int_value.null == 0)
    {
      printf("Integer: %d\n", node->int_value.value);
    }
    else
    {
      printf("Integer: null\n");
    }
    break;
  case AST_REAL:
    print_indent(indent);
    if (node->real_value.null == 0)
    {
      printf("Real: %f\n", node->real_value.value);
    }
    else
    {
      printf("Real: null\n");
    }
    break;
  case AST_CHARACTER:
    print_indent(indent);
    if (node->char_value.null == 0)
    {
      printf("Character: %c\n", node->char_value.value);
    }
    else
    {
      printf("Character: null\n");
    }
    break;
  case AST_STRING:
    print_indent(indent);
    if (node->string_value != NULL)
    {
      printf("String: \"%s\"\n", node->string_value);
    }
    else
    {
      printf("String : null\n");
    }
    break;
  case AST_BOOLEAN:
    print_indent(indent);
    if (node->boolean_value.null == 0)
    {
      printf("Boolean: %s\n", node->boolean_value.value ? "true" : "false");
    }
    else
    {
      printf("Boolean: null\n");
    }
    break;
  case AST_ARRAY:
    print_indent(indent);
    if (node->array_elements != NULL && node->array_size != 0)
    {
      printf("Array Type: %s\n", ast_type_to_string(node->array_type));

      print_indent(indent);
      printf("Array:\n");

      int i = 0;
      while (node->array_elements[i] != NULL)
      {
        print_ast(node->array_elements[i], indent + 1);
        i++;
      }

      print_indent(indent);
      printf("Array size: %d\n", node->array_size);

      print_indent(indent);
      printf("Array dimension: %d\n", node->array_dimension);
    }
    else
    {
      print_indent(indent);
      printf("Array: null\n");
    }
    break;
  case AST_RECORD:
    if (node->record_name && node->record_elements != NULL)
    {
      print_indent(indent);
      printf("Record: %s\n", node->record_name);
      print_indent(indent);
      printf("Record Elements:\n");
      int d = 0;
      while (node->record_elements[d] != NULL)
      {
        print_indent(indent + 1);
        printf("Field: %s\n", node->record_elements[d]->element_name);
        print_indent(indent + 2);
        printf("Element:\n");
        print_ast(node->record_elements[d]->element, indent + 3);
        d++;
      }
      print_indent(indent);
      printf("Number of elements: %d\n", node->field_count);
    }
    else
    {
      printf("Record: null\n");
    }

    break;
  default:
    break;
  }
}

void print_ast(ast_ *node, int indent)
{
  if (node->type == AST_NOOP)
  {
    return;
  }

  print_indent(indent);
  printf("Node Type: %s\n", ast_type_to_string(node->type));

  switch (node->type)
  {
  case AST_COMPOUND:
    print_indent(indent);
    printf("Compound:\n");
    int a = 0;
    while (node->compound_value[a] != NULL)
    {
      print_ast(node->compound_value[a], indent + 1);
      a++;
    }
    break;
  case AST_INTEGER:
  case AST_REAL:
  case AST_CHARACTER:
  case AST_STRING:
  case AST_BOOLEAN:
  case AST_ARRAY:
  case AST_RECORD:
    print_ast_literal(node, indent);
    break;
  case AST_ASSIGNMENT:
    print_indent(indent);
    printf("Assignment:\n");
    if (node->lhs)
    {
      print_indent(indent + 1);
      printf("LHS:\n");
      print_ast(node->lhs, indent + 2);
    }
    if (node->rhs)
    {
      print_indent(indent + 1);
      printf("RHS:\n");
      print_ast(node->rhs, indent + 2);
    }
    break;
  case AST_VARIABLE:
    if (node->variable_name)
    {
      print_indent(indent);
      printf("Variable: %s\n", node->variable_name);
      if (node->constant == 1)
      {
        print_indent(indent + 1);
        printf("Constant: True\n");
      }
      if (node->userinput == 1)
      {
        print_indent(indent + 1);
        printf("User Input: True\n");
      }
    }
    break;
  case AST_RECORD_ACCESS:
    if (node->variable_name)
    {
      print_indent(indent);
      printf("Record variable: %s\n", node->variable_name);
    }
    if (node->field_name)
    {
      print_indent(indent);
      printf("Record Access Field: %s\n", node->field_name);
    }
    break;
  case AST_ARRAY_ACCESS:
    print_indent(indent);
    printf("Array Access:\n");
    if (node->variable_name)
    {
      print_indent(indent + 1);
      printf("Array variable: %s\n", node->variable_name);
    }
    int b = 0;
    while (node->index[b] != NULL)
    {
      print_ast(node->index[b], indent + 1);
      b++;
    }
    break;

  case AST_INSTANTIATION:
    if (node->class_name)
    {
      print_indent(indent);
      printf("Instantiation of class: %s\n", node->class_name);
    }
    print_indent(indent);
    printf("Arguments:\n");
    int c = 0;
    while (node->arguments[c] != NULL)
    {
      print_ast(node->arguments[c], indent + 1);
      c++;
    }
    print_indent(indent);
    printf("Number of arguments: %d\n", node->arguments_count);
    break;
  case AST_ARITHMETIC_EXPRESSION:
  case AST_BOOLEAN_EXPRESSION:
    print_indent(indent);
    printf("Expression (%s):\n", node->op ? node->op : "unknown operator");
    if (node->left)
    {
      print_indent(indent + 1);
      printf("Left:\n");
      print_ast(node->left, indent + 2);
    }
    if (node->right)
    {
      print_indent(indent + 1);
      printf("Right:\n");
      print_ast(node->right, indent + 2);
    }
    break;
  case AST_RECORD_DEFINITION:
    if (node->record_name)
    {
      print_indent(indent);
      printf("Record: %s\n", node->record_name);
    }
    print_indent(indent);
    printf("Record Elements:\n");
    int d = 0;
    while (node->record_elements[d] != NULL)
    {
      print_indent(indent + 1);
      printf("Field: %s\n", node->record_elements[d]->element_name);
      print_indent(indent + 2);
      printf("Element:\n");
      print_ast(node->record_elements[d]->element, indent + 3);
      print_indent(indent + 2);
      printf("Dimension: %d\n", node->record_elements[d]->dimension);
      d++;
    }
    print_indent(indent);
    printf("Number of elements: %d\n", node->field_count);
    break;
  case AST_SUBROUTINE:
    if (node->subroutine_name)
    {
      print_indent(indent);
      printf("Subroutine: %s\n", node->subroutine_name);
    }
    print_indent(indent);
    printf("Parameters:\n");
    int e = 0;
    while (node->parameters[e] != NULL)
    {
      print_ast(node->parameters[e], indent + 1);
      e++;
    }
    print_indent(indent);
    printf("Number of parameters: %d\n", node->parameter_count);
    print_indent(indent);
    printf("Body:\n");
    int f = 0;
    while (node->body[f] != NULL)
    {
      print_ast(node->body[f], indent + 1);
      f++;
    }
    break;
  case AST_RETURN:
    print_indent(indent);
    printf("Return:\n");
    if (node->return_value)
    {
      print_ast(node->return_value, indent + 1);
    }
    break;
  case AST_OUTPUT:
    print_indent(indent);
    printf("Output Expressions:\n");
    int g = 0;
    while (node->output_expressions[g] != NULL)
    {
      print_ast(node->output_expressions[g], indent + 1);
      g++;
    }
    break;
  case AST_DEFINITE_LOOP:
    print_indent(indent);
    printf("Definite Loop:\n");
    if (node->loop_variable)
    {
      print_indent(indent + 1);
      printf("Loop Variable:\n");
      print_ast(node->loop_variable, indent + 2);
    }
    if (node->collection_expr)
    {
      print_indent(indent + 1);
      printf("Collection Variable:\n");
      print_ast(node->collection_expr, indent + 2);
    }
    if (node->end_expr)
    {
      print_indent(indent + 1);
      printf("End Expression:\n");
      print_ast(node->end_expr, indent + 2);
    }
    if (node->step_expr)
    {
      print_indent(indent + 1);
      printf("Step Expression:\n");
      print_ast(node->step_expr, indent + 2);
    }
    printf("Body:\n");
    int h = 0;
    while (node->loop_body[h] != NULL)
    {
      print_ast(node->loop_body[h], indent + 1);
      h++;
    }
    break;
  case AST_INDEFINITE_LOOP:
    print_indent(indent);
    printf("Indefinite Loop Type %s\n", node->indefinite_loop_type ? "WHILE" : "REPEAT");
    print_indent(indent);
    printf("Indefinite Loop Condition:\n");
    if (node->condition)
    {
      print_ast(node->condition, indent + 1);
    }
    print_indent(indent);
    printf("Loop Body:\n");
    int i = 0;
    while (node->loop_body[i] != NULL)
    {
      print_ast(node->loop_body[i], indent + 1);
      i++;
    }
    break;
  case AST_SELECTION:
    print_indent(indent);
    printf("IF Condition:\n");
    if (node->if_condition != NULL)
    {
      print_ast(node->if_condition, indent + 1);
    }

    print_indent(indent);
    printf("IF Body:\n");
    if (node->if_body != NULL)
    {
      int j = 0;
      while (node->if_body[j] != NULL)
      {
        print_ast(node->if_body[j], indent + 1);
        j++;
      }
    }

    // Iterate over ELSE IF conditions and bodies together
    if (node->else_if_conditions != NULL && node->else_if_bodies != NULL)
    {
      int k = 0;
      while (node->else_if_conditions[k] != NULL && node->else_if_bodies[k] != NULL)
      {
        // Print the ELSE IF condition
        print_indent(indent);
        printf("ELSE IF Condition[%d]:\n", k);
        print_ast(node->else_if_conditions[k], indent + 1);

        // Print the ELSE IF body
        print_indent(indent);
        printf("ELSE IF Body[%d]:\n", k);
        int n = 0;
        while (node->else_if_bodies[k][n] != NULL)
        {
          print_ast(node->else_if_bodies[k][n], indent + 1);
          n++;
        }
        k++;
      }
    }

    print_indent(indent);
    printf("ELSE Body:\n");
    if (node->else_body != NULL)
    {
      int l = 0;
      while (node->else_body[l] != NULL)
      {
        print_ast(node->else_body[l], indent + 1);
        l++;
      }
    }
    break;
  case AST_EXIT:
  {
    print_indent(indent);
    printf("EXIT Statement %d\n", node->exit_code);
    break;
  }
  default:
    // print_indent(indent);
    // printf("Unknown AST Node Type\n");
    break;
  }
}
