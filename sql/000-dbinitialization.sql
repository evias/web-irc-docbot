
\c postgres
drop database dbdoc;
create database dbdoc encoding 'utf-8';
\c dbdoc

create table public."user" (id_user serial primary key, login text not null, password varchar(32) not null, password_salt varchar(32) not null, date_creation timestamp not null default now(), date_update timestamp);
create table public.account_type (id_account_type serial primary key, label text not null);
create table public.user_account (id_account serial primary key, id_user integer not null references public."user" (id_user), id_account_type integer not null references public.account_type (id_account_type), date_creation timestamp not null default now(), date_update timestamp);
create table public.user_prefs (id_prefs serial primary key, id_user integer not null references public."user" (id_user), name text not null, value text not null, date_creation timestamp not null default now(), date_update timestamp);
create table public.user_profile (id_profile serial primary key, id_user integer not null references public."user" (id_user), first_name text, last_name text, email text, date_creation timestamp not null default now(), date_update timestamp);

