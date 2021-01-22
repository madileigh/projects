var btn = [];
btn.push(
    document.getElementById("about_btn"),
    document.getElementById("howto_btn"),
    document.getElementById("cust_btn"),
    document.getElementById("EA_btn"),
    document.getElementById("Est_btn"),
    document.getElementById("order_btn"),
    document.getElementById("item_btn"),
    document.getElementById("visit_btn"),
    document.getElementById("update_cust_btn"),
    document.getElementById("update_EA_btn"),
    document.getElementById("update_Est_btn"),
    document.getElementById("update_order_btn"),
    document.getElementById("update_item_btn")
    );
var popup = [];
popup.push(
    document.getElementById("about_popup"),
    document.getElementById("howto_popup"),
    document.getElementById("cust_popup"),
    document.getElementById("EA_popup"),
    document.getElementById("Est_popup"),
    document.getElementById("order_popup"),
    document.getElementById("item_popup"),
    document.getElementById("visit_popup"),
    document.getElementById("update_cust_popup"),
    document.getElementById("update_EA_popup"),
    document.getElementById("update_Est_popup"),
    document.getElementById("update_order_popup"),
    document.getElementById("update_item_popup")
    );
var span = [];
span.push(
    document.getElementsByClassName("about_close")[0],
    document.getElementsByClassName("howto_close")[0],
    document.getElementsByClassName("cust_close")[0],
    document.getElementsByClassName("EA_close")[0],
    document.getElementsByClassName("Est_close")[0],
    document.getElementsByClassName("order_close")[0],
    document.getElementsByClassName("item_close")[0],
    document.getElementsByClassName("visit_close")[0],
    document.getElementsByClassName("update_cust_close")[0],
    document.getElementsByClassName("update_EA_close")[0],
    document.getElementsByClassName("update_Est_close")[0],
    document.getElementsByClassName("update_order_close")[0],
    document.getElementsByClassName("update_item_close")[0]
    );

for (let i = 0; i < btn.length; i++) {
    console.log(i);
    btn[i].onclick = function() {
        popup[i].style.display = "block";
    }
    span[i].onclick = function() {
        popup[i].style.display = "none";
    }
    window.onclick = function(event) {
        if (event.target == popup[i]) {
            popup[i].style.display = "none";
        }
    }  
}