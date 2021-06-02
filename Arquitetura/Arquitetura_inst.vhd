	component Arquitetura is
		port (
			clk_clk       : in  std_logic                     := 'X'; -- clk
			data_a_export : out std_logic_vector(31 downto 0);        -- export
			data_b_export : out std_logic_vector(31 downto 0);        -- export
			reset_reset_n : in  std_logic                     := 'X'; -- reset_n
			wrclk_export  : out std_logic;                            -- export
			wrfull_export : in  std_logic                     := 'X'; -- export
			wrreg_export  : out std_logic                             -- export
		);
	end component Arquitetura;

	u0 : component Arquitetura
		port map (
			clk_clk       => CONNECTED_TO_clk_clk,       --    clk.clk
			data_a_export => CONNECTED_TO_data_a_export, -- data_a.export
			data_b_export => CONNECTED_TO_data_b_export, -- data_b.export
			reset_reset_n => CONNECTED_TO_reset_reset_n, --  reset.reset_n
			wrclk_export  => CONNECTED_TO_wrclk_export,  --  wrclk.export
			wrfull_export => CONNECTED_TO_wrfull_export, -- wrfull.export
			wrreg_export  => CONNECTED_TO_wrreg_export   --  wrreg.export
		);

