#pragma once

#define MAX_BUTTONS 32
#define MAX_UI_IMAGES 32
#define MAX_UI_TEXTS 64
#define MAX_UI_ELEMENTS 256

#define UI_FLAGS i32

enum Ui_Flags{
    BUTTON = 1 << 1,
    UI_TEXT = 1 << 2,
    UI_IMAGE = 1 << 3
};

struct Button{

};

struct Ui_Image{
};

struct Ui_Text{
    f32 font_size = 22;
    
    Color text_color = BLACK;
    char content[128];
};

struct Ui_Element{
    Vector2 position;
    Vector2 size;
    Vector2 pivot;

    UI_FLAGS ui_flags;

    char tag[32];
    Color color = PINK;
    
    Button button;
    Ui_Image ui_image;
    Ui_Text text;
};

struct Ui_Context{
    // Array<Button, MAX_BUTTONS>     buttons   = Array<Button, MAX_BUTTONS>();
    // Array<Ui_Image, MAX_UI_IMAGES> ui_images = Array<Ui_Image, MAX_UI_IMAGES>();
    // Array<Ui_Text, MAX_UI_TEXTS>   ui_texts  = Array<Ui_Text, MAX_UI_TEXTS>();
    Array<Ui_Element, MAX_UI_ELEMENTS> elements = Array<Ui_Element, MAX_UI_ELEMENTS>();
};

global_variable Ui_Context ui_context = {};

//all in screen space

static Ui_Element *init_ui_element(Vector2 position, Vector2 size, Vector2 pivot, Color color, const char *tag, UI_FLAGS ui_flags){
    Ui_Element new_ui_element = {position, size, pivot};
    new_ui_element.ui_flags = ui_flags;
    str_copy(new_ui_element.tag, tag);
    
    new_ui_element.color = color;
    
    ui_context.elements.add(new_ui_element);
    return ui_context.elements.last_ptr();
}

static void init_ui_text(Ui_Text *ui_text, const char *content, f32 font_size, Color text_color){
    str_copy(ui_text->content, content);
    
    ui_text->text_color = text_color;
    ui_text->font_size = font_size;
}

b32 make_button(Vector2 position, Vector2 size, Vector2 pivot, const char *text, f32 font_size, const char *tag){
    Ui_Element *new_ui_element = init_ui_element(position, size, pivot, BLACK * 0.9f, tag, BUTTON | UI_TEXT);
    init_ui_text(&new_ui_element->text, text, font_size, WHITE * 0.9f);
    
    Rectangle button_rec = {position.x - size.x * (pivot.x), position.y - size.y * pivot.y, size.x, size.y};
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), button_rec)){
        clicked_ui = true;
        return true;
    }
    
    return false;
}

void make_ui_image(Vector2 position, Vector2 size, Vector2 pivot, Color color, const char *tag){
    Ui_Element *new_ui_element = init_ui_element(position, size, pivot, color, tag, UI_IMAGE);
}

void make_ui_text(const char *content, Vector2 position, f32 font_size, Color color, const char *tag){
    Ui_Element *new_ui_element = init_ui_element(position, {100, 25}, {0, 0}, color, tag, UI_TEXT);
    init_ui_text(&new_ui_element->text, content, font_size, color);
}
