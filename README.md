# mx_chat
MXChat - Project for Computer Network HCMUS

# Members
17127004 Nguyen Vu Thu Hien

18127027 Tran Minh Duc

18127208 Ngo Thanh Phuong Thai

# Features
- [ ] Chấp nhận nhiều client 1 lúc
- [ ] Client cần đăng ký user, pass
- [ ] Tên chat là tên user
- [ ] Mọi thông điệp phải hiển thị Server
- [ ] Khi client đăng nhập hoặc đăng xuất thông báo cho server và mọi client
- [ ] Server thoát các client sẽ nhận biết ngắt kết nối
- [ ] Chat riêng tư
  - [ ] Client A gửi Client B khi đang chat riêng tư. 1 thời điểm chỉ 1 client gửi và chỉ gửi 1 file
  - [ ] Phòng chat chung, tại 1 thời điểm chỉ cho phép 1 client gửi file cùng lúc đến tất cả các client khác


# Milestone 1
- Làm quen MFC, tạo ứng dụng tính tổng 2 số nhập vào in kết quả "statictext", "richedit" (chú ý dùng hàm AfxInitRichEdit2() để init không ứng dụng compile chạy sẽ không hiện), AFXMessageBox 
- Xây dựng lớp Account (wstring username,wstring password,...) ; AccountManagement quản lý các account
    + Account chú ý làm hàm Account* Clone(){ } 
    + Các chức năng ACcountManagement
      + vector<Account> ReadFromFile(wstring path);
      + void AddAccount(wstring path, Account account);
      + void RemoveAccount(wstring path, Account account );
      + int CheckAccount(Account account)
          + 0 : Không tồn tại
          + 1 : Tồn tại nhưng mật khẩu sai
          + 2 : Tồn tại và mật khẩu đúng 
- Thiết kế form đăng nhập. Form chọn chức năng chat riêng tư, form chat riêng tư ,form chat nhóm 
- Nghiên cứu cách ấn button tạo form mới, viết hàm tạo form mới
- Nghiên cứu cách xoá các control trong form hiện tại ( ví dụ sau khi đăng nhập thì form username và password sẽ mất đi để hiển thị nội dung các chức năng)
- Nghiên cứu openfiledialog tương tự c# trong MFC

