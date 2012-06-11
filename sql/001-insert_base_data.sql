
BEGIN ;

truncate public.user_account cascade;
truncate public.user_profile cascade;
truncate public.user_prefs cascade;
truncate public."user" cascade;
truncate public.account_type cascade;

-- account types
insert into public.account_type
    (label) values ('Administrator'), ('Standard');

insert into public."user"
    (login, password, password_salt)
    values
        ('greg', md5('opendev' || md5('opensalt')), md5('opensalt')),
        ('sophie', md5('opendev' || md5('opensalt')), md5('opensalt'));

insert into public.user_account
    (id_user, id_account_type)
    values
        ((select id_user from public."user" where login = 'greg'), (select id_account_type from public.account_type where label = 'Administrator')),
        ((select id_user from public."user" where login = 'sophie'), (select id_account_type from public.account_type where label = 'Standard'));

