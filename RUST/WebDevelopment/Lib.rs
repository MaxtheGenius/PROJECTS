use yew::prelude::*;
use wasm_bindgen::prelude::*;
use web_sys::Window;
use serde::{Deserialize, Serialize};
use gloo_net::http::Request;
use yew::services::ConsoleService;

#[derive(Serialize, Deserialize, Clone, Debug)]
struct Document {
    id: u32,
    title: String,
    content: String,
    last_edited_by: String,
}

#[function_component(App)]
fn app() -> Html {
    let documents = use_state(|| Vec::<Document>::new());
    let title = use_state(|| String::new());
    let content = use_state(|| String::new());
    let editor_name = use_state(|| String::new());

    let on_title_change = Callback::from(move |e: InputEvent| {
        let value = e.target_unchecked_into::<web_sys::HtmlInputElement>().value();
        title.set(value);
    });

    let on_content_change = Callback::from(move |e: InputEvent| {
        let value = e.target_unchecked_into::<web_sys::HtmlTextAreaElement>().value();
        content.set(value);
    });

    let on_editor_name_change = Callback::from(move |e: InputEvent| {
        let value = e.target_unchecked_into::<web_sys::HtmlInputElement>().value();
        editor_name.set(value);
    });

    let fetch_documents = {
        let documents = documents.clone();
        Callback::from(move |_| {
            wasm_bindgen_futures::spawn_local(async move {
                match Request::get("/api/documents")
                    .send()
                    .await
                {
                    Ok(response) => {
                        let docs: Vec<Document> = response.json().await.unwrap();
                        documents.set(docs);
                    }
                    Err(e) => ConsoleService::log(&format!("Failed to fetch documents: {}", e)),
                }
            });
        })
    };

    let create_document = {
        let title = title.clone();
        let content = content.clone();
        let editor_name = editor_name.clone();
        Callback::from(move |_| {
            wasm_bindgen_futures::spawn_local(async move {
                let new_doc = Document {
                    id: rand::random::<u32>(), // Simulating random ID generation
                    title: title.clone(),
                    content: content.clone(),
                    last_edited_by: editor_name.clone(),
                };

                match Request::post("/api/create")
                    .json(&new_doc)
                    .send()
                    .await
                {
                    Ok(response) => {
                        let created_doc: Document = response.json().await.unwrap();
                        ConsoleService::log(&format!("Document created: {:?}", created_doc));
                    }
                    Err(e) => ConsoleService::log(&format!("Failed to create document: {}", e)),
                }
            });
        })
    };

    html! {
        <div>
            <h1>{ "Collaborative Document Editing" }</h1>
            <input type="text" placeholder="Editor Name" oninput={on_editor_name_change} />
            <input type="text" placeholder="Document Title" value={(*title).clone()} oninput={on_title_change} />
            <textarea placeholder="Document Content" value={(*content).clone()} oninput={on_content_change}></textarea>
            <button onclick={create_document}>{ "Create Document" }</button>
            
            <h2>{ "Existing Documents" }</h2>
            <ul>
                { for (*documents).iter().map(|doc| {
                    html! {
                        <li key={doc.id}>
                            <strong>{ &doc.title }</strong> - { &doc.content } 
                            <small>{ format!(" (Last edited by: {})", doc.last_edited_by) }</small>
                        </li>
                    }
                })}
            </ul>
        </div>
    }
}

#[wasm_bindgen(start)]
pub fn run_app() {
    yew::start_app::<App>();
}