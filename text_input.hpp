#pragma once

#define INPUT_FIELD_MAX_CHARS 100

struct Input_Field{
    Vector2 position;
    Vector2 size;
    //Vector2 pivot;
    
    char content[INPUT_FIELD_MAX_CHARS];
    char tag[16];
    int chars_count = 0;
    
    f32 font_size = 22;
    
    b32 in_focus = false;
};

global_variable Array<Input_Field> input_fields = Array<Input_Field>(16);
global_variable Input_Field focus_input_field;

void update_input_field(){
    if (focus_input_field.in_focus && IsKeyPressed(KEY_ESCAPE)){
        focus_input_field.in_focus = false;
    }

    if (focus_input_field.in_focus)
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (focus_input_field.chars_count < INPUT_FIELD_MAX_CHARS))
            {
                focus_input_field.content[focus_input_field.chars_count] = (char)key;
                focus_input_field.content[focus_input_field.chars_count+1] = '\0';
                focus_input_field.chars_count++;
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            focus_input_field.chars_count--;
            if (focus_input_field.chars_count < 0) focus_input_field.chars_count = 0;
            focus_input_field.content[focus_input_field.chars_count] = '\0';
        }
    } else{
         SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

// b32 is_input_field_exists(Input_Field input_field){
//     for (int i = 0; i < input_fields.count; i++){
//         if (input_field.id == input_fields.get(i).id){
//             return true;
//         }
//     }
    
//     return false;
// }

void make_last_in_focus(){
    input_fields.last_ptr()->in_focus = true;
    focus_input_field = input_fields.last();
}

void copy_input_field(Input_Field *dest, Input_Field *src){
    str_copy(dest->content, src->content);
    str_copy(dest->tag,     src->tag);
    dest->chars_count = src->chars_count;
    dest->in_focus = src->in_focus;
}

b32 make_input_field(const char *content, Vector2 position, Vector2 size, const char *tag){
    //means it's the same and we want to keep all contents
    Input_Field input_field = {position, size};
    if (focus_input_field.in_focus && str_cmp(focus_input_field.tag, tag)){
        input_field.in_focus = true;
        copy_input_field(&input_field, &focus_input_field);
        
        input_fields.add(input_field);
        
        if (IsKeyPressed(KEY_ENTER)){
            focus_input_field.in_focus = false;
            return true;
        }
    
        return false;
    } else{
        str_copy(input_field.content, content);
        input_field.chars_count = str_len(content);
        str_copy(input_field.tag, tag);
    }
    
    
    Rectangle field_rect = {input_field.position.x, input_field.position.y, input_field.size.x, input_field.size.y};
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), field_rect)){
        input_field.in_focus = true;
        copy_input_field(&focus_input_field, &input_field);
    }
    
    input_fields.add(input_field);
    
    return false;
}