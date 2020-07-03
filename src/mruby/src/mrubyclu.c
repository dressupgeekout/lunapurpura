/*
 * mruby bindings to Luna Purpura's CLU library
 */

#include <mruby.h>
#include <mruby/numeric.h>

#include <lputil.h>

#include <clu.h>

static mrb_value mrb_clu_new_from_file(mrb_state *mrb, mrb_value self);
static mrb_value mrb_clu_color_at_index(mrb_state *mrb, mrb_value self);
static mrb_value mrb_clu_get_name(mrb_state *mrb, mrb_value self);

/* */

/* XXX IMPLEMENT ME */
/*
 * CLU.new_from_file(path) # => clu
 */
static mrb_value
mrb_clu_new_from_file(mrb_state *mrb, mrb_value self)
{
	char *path = NULL;

	mrb_get_args(mrb, "z", &path);

	LPStatus status;
	CLU **clu = mrb_malloc(mrb, sizeof(CLU)); // XXX
	*clu = CLU_NewFromFile(path, &status); /* XXX check status */
	/* XXX return object of class CLU */
	return mrb_nil_value();
}

/* XXX IMPLEMENT ME */
/*
 * clu.color_at_index(index) # => [r, g, b]
 */
static mrb_value
mrb_clu_color_at_index(mrb_state *mrb, mrb_value self)
{
	mrb_int index;
	mrb_get_args(mrb, "i", &index);
	//return mrb_nil_value();
	return mrb_int_value(mrb, index);
}

/* XXX IMPLEMENT ME */
/*
 * clu.get_name # => String
 */
static mrb_value
mrb_clu_get_name(mrb_state *mrb, mrb_value self)
{
	return mrb_str_new_cstr(mrb, "XXXFIXME");
}

/* */

void
mrb_mruby_clu_gem_init(mrb_state *mrb)
{
	struct RClass *clu_class;
	clu_class = mrb_define_class(mrb, "CLU", mrb->object_class);

	mrb_define_class_method(mrb, clu_class, "new_from_file", mrb_clu_new_from_file, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, clu_class, "color_at_index", mrb_clu_color_at_index, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, clu_class, "get_name", mrb_clu_get_name, MRB_ARGS_REQ(1));
}

void
mrb_mruby_clu_gem_final(mrb_state *mrb)
{
	/* OK */ //free something maybe
}
